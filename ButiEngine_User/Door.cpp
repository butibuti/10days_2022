#include "stdafx_u.h"
#include "Door.h"
#include "InputManager.h"
#include "ButiBulletWrap/ButiBulletWrap/Common.h"

void ButiEngine::Door::OnUpdate()
{
	//Playerが触れている状態で決定キーを押したら次のシーンへ移行する
	if (m_isHitPlayer && InputManager::IsTriggerDecideKey())
	{
		ChangeScene();
	}
}

void ButiEngine::Door::OnSet()
{
	gameObject.lock()->AddCollisionEnterReaction(
		[this](ButiBullet::ContactData& arg_other) 
		{
			if (arg_other.vwp_gameObject.lock()) 
			{
				if (arg_other.vwp_gameObject.lock()->HasGameObjectTag("Player"))
				{
					m_isHitPlayer = true;
				}
			}
		}
	);

	gameObject.lock()->AddCollisionLeaveReaction(
		[this](ButiBullet::ContactData& arg_other)
		{
			if (arg_other.vwp_gameObject.lock())
			{
				if (arg_other.vwp_gameObject.lock()->HasGameObjectTag("Player"))
				{
					m_isHitPlayer = false;
				}
			}
		}
	);
}

void ButiEngine::Door::Start()
{
}

void ButiEngine::Door::OnRemove()
{
}

void ButiEngine::Door::OnShowUI()
{
	GUI::Text("NextSceneName:" + m_nextSceneName);

	static char nextSceneNameBuff[128];
	GUI::InputTextWithHint("##nextSceneName", m_nextSceneName, nextSceneNameBuff, sizeof(nextSceneNameBuff));
	if (GUI::Button("Set"))
	{
		m_nextSceneName = nextSceneNameBuff;
		memset(nextSceneNameBuff, 0, 128);
	}

}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Door::Clone()
{
	auto output = ObjectFactory::Create<Door>();
	output->m_nextSceneName = m_nextSceneName;
	return output;
}

void ButiEngine::Door::ChangeScene()
{
	if (m_nextSceneName == "") { return; }

	auto sceneManager = gameObject.lock()->GetApplication().lock()->GetSceneManager();
	sceneManager->RemoveScene(m_nextSceneName);
	sceneManager->LoadScene(m_nextSceneName);
	sceneManager->ChangeScene(m_nextSceneName);
}
