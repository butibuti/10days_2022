#include "stdafx_u.h"
#include "GunAction_BossLauncher.h"
#include "EquipGun.h"
#include "Gun.h"
#include "SeparateDrawObject.h"
#include "BossEnemy.h"
#include "Header/GameObjects/DefaultGameComponent/RigidBodyComponent.h"
#include "Header/GameObjects/DefaultGameComponent/PositionAnimationComponent.h"
#include "Header/GameObjects/DefaultGameComponent/RotationAnimationComponent.h"

void ButiEngine::GunAction_BossLauncher::OnUpdate()
{
	if (m_vwp_bossEnemyComponent.lock()->IsPause())
	{
		return;
	}

	switch (m_phase)
	{
	case ButiEngine::GunAction_BossLauncherPhase::LockOn:
		break;
	case ButiEngine::GunAction_BossLauncherPhase::Shoot:
		break;
	case ButiEngine::GunAction_BossLauncherPhase::Rest:
		break;
	case ButiEngine::GunAction_BossLauncherPhase::AfterShoot:
		break;
	default:
		break;
	}
}

void ButiEngine::GunAction_BossLauncher::OnSet()
{
	m_vwp_drawObject = gameObject.lock()->GetGameComponent<SeparateDrawObject>()->GetDrawObject();
	//m_vwp_drawObject.lock()->GetGameComponent<LookAtComponent>()->SetIsActive(false);

	m_vwp_bossEnemyComponent = gameObject.lock()->GetGameComponent<BossEnemy>();
	m_vwp_bossEnemyComponent.lock()->StartGunAction();

	m_vwp_gunComponent = m_vwp_bossEnemyComponent.lock()->ChangeGun("Gun_Boss_Launcher");

	m_lockOnTime = 100;
	m_restTime = 30;
	m_afterTime = 30;
	m_actionTime = 10;
	m_vlp_waitTimer = ObjectFactory::Create<RelativeTimer>(m_lockOnTime);
	m_vlp_actionTimer = ObjectFactory::Create<RelativeTimer>(m_actionTime);
	m_shootCount = 0;
	m_shootCountMax = 3;
}

void ButiEngine::GunAction_BossLauncher::Start()
{
}

void ButiEngine::GunAction_BossLauncher::OnRemove()
{
}

void ButiEngine::GunAction_BossLauncher::OnShowUI()
{
	GUI::BulletText(u8"waitTimer");
	GUI::Text(m_vlp_waitTimer->GetCurrentCountFrame());
	GUI::BulletText(u8"actionTimer");
	GUI::Text(m_vlp_actionTimer->GetCurrentCountFrame());
	GUI::BulletText(u8"shootCount");
	GUI::Text(m_shootCount);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::GunAction_BossLauncher::Clone()
{
	return ObjectFactory::Create<GunAction_BossLauncher>();
}

void ButiEngine::GunAction_BossLauncher::StartPause()
{
}

void ButiEngine::GunAction_BossLauncher::FinishPause()
{
}

void ButiEngine::GunAction_BossLauncher::StartLockOnPhase()
{
	m_phase = GunAction_BossLauncherPhase::LockOn;
}

void ButiEngine::GunAction_BossLauncher::UpdateLockOnPhase()
{
}

void ButiEngine::GunAction_BossLauncher::StartShootPhase()
{
	m_phase = GunAction_BossLauncherPhase::Shoot;
}

void ButiEngine::GunAction_BossLauncher::UpdateShootPhase()
{
}

void ButiEngine::GunAction_BossLauncher::StartRestPhase()
{
	m_phase = GunAction_BossLauncherPhase::Rest;
}

void ButiEngine::GunAction_BossLauncher::UpdateRestPhase()
{
}

void ButiEngine::GunAction_BossLauncher::StartAfterShootPhase()
{
	m_phase = GunAction_BossLauncherPhase::AfterShoot;
}

void ButiEngine::GunAction_BossLauncher::UpdateAfterShootPhase()
{
}

void ButiEngine::GunAction_BossLauncher::Rotate()
{
}
