#include "stdafx_u.h"
#include "Door.h"

void ButiEngine::Door::OnUpdate()
{
	//Player���G��Ă����ԂŌ���L�[���������玟�̃V�[���ֈڍs����
}

void ButiEngine::Door::OnSet()
{
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
