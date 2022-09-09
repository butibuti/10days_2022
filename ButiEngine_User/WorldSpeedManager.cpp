#include "stdafx_u.h"
#include "WorldSpeedManager.h"

ButiEngine::Value_ptr<ButiEngine::Timer> ButiEngine::WorldSpeedManager::g_vlp_timer;
bool ButiEngine::WorldSpeedManager::g_isChangeSpeed;

void ButiEngine::WorldSpeedManager::OnUpdate()
{
	if (g_isChangeSpeed)
	{
		OnChangeSpeed();
	}
}

void ButiEngine::WorldSpeedManager::OnSet()
{
}

void ButiEngine::WorldSpeedManager::OnShowUI()
{
	GUI::BulletText("GetWorldSpeed()");
	auto temp = GameDevice::GetWorldSpeed();
	if (GUI::DragFloat("##speed", temp, 0.01f, 0.0f, 1.0f)) {
		GameDevice::SetWorldSpeed(temp);
	}

}

void ButiEngine::WorldSpeedManager::Start()
{
	g_vlp_timer = ObjectFactory::Create<AbsoluteTimer>(0);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::WorldSpeedManager::Clone()
{
	return ObjectFactory::Create<WorldSpeedManager>();
}

void ButiEngine::WorldSpeedManager::OnChangeSpeed()
{
	if (g_vlp_timer->Update())
	{
		GameDevice::SetWorldSpeed(1.0f);

		g_vlp_timer->Stop();
		g_vlp_timer->Reset();

		g_isChangeSpeed = false;
	}
}
