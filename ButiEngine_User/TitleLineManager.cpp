#include "stdafx_u.h"
#include "TitleLineManager.h"
#include "TitleLine.h"

void ButiEngine::TitleLineManager::OnUpdate()
{
	if (m_vlp_timer->Update())
	{
		if (m_isAppear)
		{
			if (m_titleLineNum < m_vec_rightTitleLineComponents.size())
			{
				m_vec_rightTitleLineComponents[m_titleLineNum].lock()->Appear();
			}
			if (m_titleLineNum < m_vec_leftTitleLineComponents.size())
			{
				m_vec_leftTitleLineComponents[m_titleLineNum].lock()->Appear();
			}
		}

		m_titleLineNum++;
		if (m_titleLineNum >= m_vec_rightTitleLineComponents.size() && m_titleLineNum >= m_vec_leftTitleLineComponents.size())
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
		if((*itr)->transform->GetLocalPosition().x < 0.0f)
		{
			m_vec_leftTitleLineComponents.push_back((*itr)->GetGameComponent<TitleLine>());
		}
		else
		{
			m_vec_rightTitleLineComponents.push_back((*itr)->GetGameComponent<TitleLine>());
		}
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

	{
		auto end = m_vec_rightTitleLineComponents.end();
		for (auto itr = m_vec_rightTitleLineComponents.begin(); itr != end; ++itr)
		{
			(*itr).lock()->Disappear();
		}
	}
	{
		auto end = m_vec_leftTitleLineComponents.end();
		for (auto itr = m_vec_leftTitleLineComponents.begin(); itr != end; ++itr)
		{
			(*itr).lock()->Disappear();
		}
	}

}
