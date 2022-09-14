#include "stdafx_u.h"
#include "GameClearManager.h"
#include "InputManager.h"

void ButiEngine::GameClearManager::OnUpdate()
{
	if (m_vlp_waitTimer->Update())
	{
		m_vlp_waitTimer->Stop();
		GetManager().lock()->AddObjectFromCereal("GameClear");
	}

	if (m_vlp_waitTimer->IsOn())
	{
		return;
	}

	if (!m_vlp_timer->IsOn() && InputManager::IsTriggerDecideKey())
	{
		m_vlp_timer->Start();
		GetManager().lock()->AddObjectFromCereal("FadeBlack");
	}

	if (m_vlp_timer->Update())
	{
		m_vlp_timer->Stop();

		auto sceneManager = gameObject.lock()->GetApplication().lock()->GetSceneManager();
		std::string sceneName = "TitleScene";
		sceneManager->RemoveScene(sceneName);
		sceneManager->LoadScene(sceneName);
		sceneManager->ChangeScene(sceneName);
	}
}

void ButiEngine::GameClearManager::OnSet()
{
}

void ButiEngine::GameClearManager::Start()
{
	m_vlp_waitTimer = ObjectFactory::Create<RelativeTimer>(180);
	m_vlp_timer = ObjectFactory::Create<RelativeTimer>(120);
	
	m_vlp_waitTimer->Start();
	PlayBGM("Sound/GameClearBGM.wav", 0.1f);
}

void ButiEngine::GameClearManager::OnRemove()
{
}

void ButiEngine::GameClearManager::OnShowUI()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::GameClearManager::Clone()
{
	return ObjectFactory::Create<GameClearManager>();
}
