#include "stdafx_u.h"
#include "GameOverManager.h"
#include "InputManager.h"

void ButiEngine::GameOverManager::OnUpdate()
{
	if (m_vlp_waitTimer->Update())
	{
		m_vlp_waitTimer->Stop();
		GetManager().lock()->AddObjectFromCereal("GameOver");
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

void ButiEngine::GameOverManager::OnSet()
{
}

void ButiEngine::GameOverManager::Start()
{
	m_vlp_waitTimer = ObjectFactory::Create<RelativeTimer>(180);
	m_vlp_timer = ObjectFactory::Create<RelativeTimer>(120);

	m_vlp_waitTimer->Start();
}

void ButiEngine::GameOverManager::OnRemove()
{
}

void ButiEngine::GameOverManager::OnShowUI()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::GameOverManager::Clone()
{
	return ObjectFactory::Create<GameOverManager>();
}
