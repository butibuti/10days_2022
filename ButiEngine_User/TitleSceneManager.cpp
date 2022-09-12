#include "stdafx_u.h"
#include "TitleSceneManager.h"
#include "TitleLineManager.h"
#include "InputManager.h"

void ButiEngine::TitleSceneManager::OnUpdate()
{
	if (!m_vlp_endTimer->IsOn() && InputManager::IsTriggerDecideKey())
	{
		GetManager().lock()->GetGameObject("TitleLineManager").lock()->GetGameComponent<TitleLineManager>()->Disappear();
		m_vlp_endTimer->Start();
	}

	if (m_vlp_endTimer->Update())
	{
		m_vlp_endTimer->Stop();
	}
}

void ButiEngine::TitleSceneManager::OnSet()
{
}

void ButiEngine::TitleSceneManager::Start()
{
	GetManager().lock()->GetGameObject("TitleLineManager").lock()->GetGameComponent<TitleLineManager>()->Appear();

	m_vlp_endTimer = ObjectFactory::Create<RelativeTimer>(120);
}

void ButiEngine::TitleSceneManager::OnRemove()
{
}

void ButiEngine::TitleSceneManager::OnShowUI()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::TitleSceneManager::Clone()
{
	return ObjectFactory::Create<TitleSceneManager>();
}
