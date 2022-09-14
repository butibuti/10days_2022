#include "stdafx_u.h"
#include "TitleSceneManager.h"
#include "TitleObjectManager.h"
#include "InputManager.h"

void ButiEngine::TitleSceneManager::OnUpdate()
{
	if (m_vlp_waitTimer->Update())
	{
		m_vlp_waitTimer->Stop();
		GetManager().lock()->GetGameObject("TitleObjectManager").lock()->GetGameComponent<TitleObjectManager>()->AppearLine();
	}

	if (m_vlp_startTimer->Update())
	{
		m_vlp_startTimer->Stop();
	}
	if (m_vlp_startTimer->IsOn())
	{
		return;
	}

	if (!m_vlp_endTimer->IsOn() && InputManager::IsTriggerDecideKey())
	{
		GetManager().lock()->GetGameObject("TitleObjectManager").lock()->GetGameComponent<TitleObjectManager>()->Disappear();
		m_vlp_endTimer->Start();
		PlaySE("Sound/Decide.wav", 0.05f);
	}

	if (m_vlp_endTimer->Update())
	{
		m_vlp_endTimer->Stop();
		auto sceneManager = gameObject.lock()->GetApplication().lock()->GetSceneManager();
		std::string sceneName = "Stage_1";
		sceneManager->RemoveScene(sceneName);
		sceneManager->LoadScene(sceneName);
		sceneManager->ChangeScene(sceneName);
	}
}

void ButiEngine::TitleSceneManager::OnSet()
{
}

void ButiEngine::TitleSceneManager::Start()
{
	GetManager().lock()->GetGameObject("TitleObjectManager").lock()->GetGameComponent<TitleObjectManager>()->AppearHexagon();

	m_vlp_waitTimer = ObjectFactory::Create<RelativeTimer>(50);
	m_vlp_waitTimer->Start();

	m_vlp_startTimer = ObjectFactory::Create<RelativeTimer>(90);
	m_vlp_startTimer->Start();

	m_vlp_endTimer = ObjectFactory::Create<RelativeTimer>(120);
	PlayBGM("Sound/TitleBGM.wav", 0.1f);
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
