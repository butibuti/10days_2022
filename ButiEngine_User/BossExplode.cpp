#include "stdafx_u.h"
#include "BossExplode.h"
#include "WhiteOut.h"
#include "Camera.h"
#include "SeparateDrawObject.h"
#include "EquipGun.h"

void ButiEngine::BossExplode::OnUpdate()
{
	if (m_vlp_waitTimer->Update())
	{
		m_vlp_waitTimer->Stop();
		GetManager().lock()->GetGameObject("CameraMan").lock()->GetGameComponent<Camera>()->StartShake(250, Vector3(0.1f, 0.1f, 0.0f), 0);
		m_vlp_timer->Start();
	}

	if (m_vlp_timer->Update())
	{
		auto white = GetManager().lock()->AddObjectFromCereal("White");
		white.lock()->GetGameComponent<WhiteOut>()->SetLife(40);
		m_loopCount++;
		if (m_loopCount >= 3)
		{
			m_vlp_timer->Stop();
			white.lock()->GetGameComponent<WhiteOut>()->SetLife(120);
			gameObject.lock()->GetGameComponent<SeparateDrawObject>()->Dead();
			gameObject.lock()->GetGameComponent<EquipGun>()->Dead();
			gameObject.lock()->SetIsRemove(true);
			GetManager().lock()->AddObjectFromCereal("GameClearManager");
		}
	}
}

void ButiEngine::BossExplode::OnSet()
{
	m_vlp_waitTimer = ObjectFactory::Create<RelativeTimer>(60);
	m_vlp_waitTimer->Start();
	m_vlp_timer = ObjectFactory::Create<RelativeTimer>(90);
	m_loopCount = 0;
}

void ButiEngine::BossExplode::Start()
{
}

void ButiEngine::BossExplode::OnRemove()
{
}

void ButiEngine::BossExplode::OnShowUI()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::BossExplode::Clone()
{
	return ObjectFactory::Create<BossExplode>();
}
