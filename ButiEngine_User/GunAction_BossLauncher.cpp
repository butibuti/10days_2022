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
		UpdateLockOnPhase();
		break;
	case ButiEngine::GunAction_BossLauncherPhase::Shoot:
		UpdateShootPhase();
		break;
	case ButiEngine::GunAction_BossLauncherPhase::AfterShoot:
		UpdateAfterShootPhase();
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

	m_lockOnTime = 80;
	m_afterTime = 30;
	m_actionTime = 200;
	m_vlp_waitTimer = ObjectFactory::Create<RelativeTimer>(m_lockOnTime);
	m_vlp_actionTimer = ObjectFactory::Create<RelativeTimer>(m_actionTime);

	StartLockOnPhase();
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
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::GunAction_BossLauncher::Clone()
{
	return ObjectFactory::Create<GunAction_BossLauncher>();
}

void ButiEngine::GunAction_BossLauncher::StartPause()
{
	m_vlp_waitTimer->Stop();
	m_vlp_actionTimer->Stop();
	m_vwp_gunComponent.lock()->ShootStop();
	m_vwp_drawObject.lock()->GetGameComponent<LookAtComponent>()->SetIsActive(false);
}

void ButiEngine::GunAction_BossLauncher::FinishPause()
{
	m_vlp_waitTimer->Start();
	m_vlp_actionTimer->Start();
	if (m_phase == GunAction_BossLauncherPhase::Shoot)
	{
		m_vwp_gunComponent.lock()->ShootStart();
	}

	m_vwp_drawObject.lock()->GetGameComponent<LookAtComponent>()->SetIsActive(true);
}

void ButiEngine::GunAction_BossLauncher::StartLockOnPhase()
{
	m_phase = GunAction_BossLauncherPhase::LockOn;
	m_vlp_waitTimer->Reset();
	m_vlp_waitTimer->Start();
}

void ButiEngine::GunAction_BossLauncher::UpdateLockOnPhase()
{
	if (m_vlp_waitTimer->Update())
	{
		StartShootPhase();
	}
}

void ButiEngine::GunAction_BossLauncher::StartShootPhase()
{
	m_phase = GunAction_BossLauncherPhase::Shoot;
	m_vlp_waitTimer->Stop();
	m_vlp_actionTimer->ChangeCountFrame(m_actionTime);
	m_vlp_actionTimer->Reset();
	m_vlp_actionTimer->Start();
	m_vwp_gunComponent.lock()->ShootStart();
}

void ButiEngine::GunAction_BossLauncher::UpdateShootPhase()
{
	Rotate();

	if (m_vlp_actionTimer->Update())
	{
		m_vwp_gunComponent.lock()->ShootStop();

		StartAfterShootPhase();
	}
}

void ButiEngine::GunAction_BossLauncher::StartAfterShootPhase()
{
	m_phase = GunAction_BossLauncherPhase::AfterShoot;
	m_vlp_actionTimer->Stop();
	m_vlp_waitTimer->ChangeCountFrame(m_afterTime);
	m_vlp_waitTimer->Reset();
	m_vlp_waitTimer->Start();
}

void ButiEngine::GunAction_BossLauncher::UpdateAfterShootPhase()
{
	if (m_vlp_waitTimer->Update())
	{
		//I—¹ˆ—
		m_vwp_bossEnemyComponent.lock()->FinishGunAction();
		m_vwp_drawObject.lock()->GetGameComponent<LookAtComponent>()->SetIsActive(true);
		m_vwp_bossEnemyComponent.lock()->ChangeGun("Gun_Boss_Normal");
		SetIsRemove(true);
	}
}

void ButiEngine::GunAction_BossLauncher::Rotate()
{
}
