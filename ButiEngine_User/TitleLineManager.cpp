#include "stdafx_u.h"
#include "TitleLineManager.h"
#include "TitleLine.h"

void ButiEngine::TitleLineManager::OnUpdate()
{
	if (m_vlp_timer->Update())
	{
		if (m_isAppear)
		{
			m_vec_titleLineComponents[m_titleLineNum].lock()->Appear();
		}
		//else if (m_isDisappear)
		//{
		//	m_vec_titleLineComponents[m_titleLineNum].lock()->Disappear();
		//}

		m_titleLineNum++;
		if (m_titleLineNum >= m_vec_titleLineComponents.size())
		{
			m_vlp_timer->Stop();
			m_isAppear = false;
			m_isDisappear = false;
		}
	}
}

void ButiEngine::TitleLineManager::OnSet()
{
}

void ButiEngine::TitleLineManager::Start()
{
	auto titleLines = GetManager().lock()->GetGameObjects(GameObjectTag("TitleLine"));
	auto end = titleLines.end();
	for (auto itr = titleLines.begin(); itr != end; ++itr)
	{
		m_vec_titleLineComponents.push_back((*itr)->GetGameComponent<TitleLine>());
	}

	m_vlp_timer = ObjectFactory::Create<RelativeTimer>(1);

	m_titleLineNum = 0;
	m_isAppear = false;
	m_isDisappear = false;
}

void ButiEngine::TitleLineManager::OnRemove()
{
}

void ButiEngine::TitleLineManager::OnShowUI()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::TitleLineManager::Clone()
{
	return ObjectFactory::Create<TitleLineManager>();
}

void ButiEngine::TitleLineManager::Appear()
{
	m_isAppear = true;
	m_titleLineNum = 0;
	m_vlp_timer->Start();
}

void ButiEngine::TitleLineManager::Disappear()
{
	//m_isDisappear = true;
	//m_titleLineNum = 0;
	//m_vlp_timer->Start();

	auto end = m_vec_titleLineComponents.end();
	for (auto itr = m_vec_titleLineComponents.begin(); itr != end; ++itr)
	{
		(*itr).lock()->Disappear();
	}
}
