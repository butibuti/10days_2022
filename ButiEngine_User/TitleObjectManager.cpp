#include "stdafx_u.h"
#include "TitleObjectManager.h"
#include "TitleLine.h"
#include "Hexagon.h"

void ButiEngine::TitleObjectManager::OnUpdate()
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

void ButiEngine::TitleObjectManager::OnSet()
{
}

void ButiEngine::TitleObjectManager::Start()
{
	{
		auto titleLines = GetManager().lock()->GetGameObjects(GameObjectTag("TitleLine"));
		auto end = titleLines.end();
		for (auto itr = titleLines.begin(); itr != end; ++itr)
		{
			if ((*itr)->transform->GetLocalPosition().x < 0.0f)
			{
				m_vec_leftTitleLineComponents.push_back((*itr)->GetGameComponent<TitleLine>());
			}
			else
			{
				m_vec_rightTitleLineComponents.push_back((*itr)->GetGameComponent<TitleLine>());
			}
		}
	}

	{
		auto hexagons = GetManager().lock()->GetGameObjects(GameObjectTag("Hexagon"));
		auto end = hexagons.end();
		for (auto itr = hexagons.begin(); itr != end; ++itr)
		{
			m_vec_hexagonComponents.push_back((*itr)->GetGameComponent<Hexagon>());
		}
	}

	m_vlp_timer = ObjectFactory::Create<RelativeTimer>(1);

	m_titleLineNum = 0;
	m_isAppear = false;
	m_isDisappear = false;
}

void ButiEngine::TitleObjectManager::OnRemove()
{
}

void ButiEngine::TitleObjectManager::OnShowUI()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::TitleObjectManager::Clone()
{
	return ObjectFactory::Create<TitleObjectManager>();
}

void ButiEngine::TitleObjectManager::AppearLine()
{
	m_isAppear = true;
	m_titleLineNum = 0;
	m_vlp_timer->Start();
}

void ButiEngine::TitleObjectManager::AppearHexagon()
{
	auto end = m_vec_hexagonComponents.end();
	for (auto itr = m_vec_hexagonComponents.begin(); itr != end; ++itr)
	{
		(*itr).lock()->Appear();
	}
}

void ButiEngine::TitleObjectManager::Disappear()
{
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
	{
		auto end = m_vec_hexagonComponents.end();
		for (auto itr = m_vec_hexagonComponents.begin(); itr != end; ++itr)
		{
			(*itr).lock()->Disappear();
		}
	}

}
