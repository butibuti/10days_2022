#include "stdafx_u.h"
#include "GunAction_BossSpin.h"
#include "EquipGun.h"
#include "Gun.h"
#include "SeparateDrawObject.h"
#include "BossEnemy.h"
#include "Header/GameObjects/DefaultGameComponent/RigidBodyComponent.h"
#include "Header/GameObjects/DefaultGameComponent/PositionAnimationComponent.h"
#include "Header/GameObjects/DefaultGameComponent/RotationAnimationComponent.h"

void ButiEngine::GunAction_BossSpin::OnUpdate()
{
	if (m_vwp_bossEnemyComponent.lock()->IsPause())
	{
		return;
	}

	switch (m_phase)
	{
	case ButiEngine::GunAction_BossSpinPhase::BeforeShoot:
		UpdateBeforeShootPhase();
		break;
	case ButiEngine::GunAction_BossSpinPhase::Shoot:
		UpdateShootPhase();
		break;
	case ButiEngine::GunAction_BossSpinPhase::AfterShoot:
		UpdateAfterShootPhase();
		break;
	default:
		break;
	}
}

void ButiEngine::GunAction_BossSpin::OnSet()
{
	m_vwp_drawObject = gameObject.lock()->GetGameComponent<SeparateDrawObject>()->GetDrawObject();
	m_vwp_drawObject.lock()->GetGameComponent<LookAtComponent>()->SetIsActive(false);

	m_vwp_bossEnemyComponent = gameObject.lock()->GetGameComponent<BossEnemy>();
	m_vwp_bossEnemyComponent.lock()->StartGunAction();

	m_vwp_rightGunComponent = m_vwp_bossEnemyComponent.lock()->ChangeGun("Gun_Boss_Spin_Right");
	m_vwp_leftGunComponent = gameObject.lock()->GetGameComponent<EquipGun>()->AddGun("Gun_Boss_Spin_Left").lock()->GetGameComponent<Gun>();

	m_waitTime = 20;
	m_actionTime = 300;
	m_vlp_waitTimer = ObjectFactory::Create<RelativeTimer>(m_waitTime);
	m_vlp_actionTimer = ObjectFactory::Create<RelativeTimer>(m_actionTime);
	m_spinCount = 5;

	StartBeforeShootPhase();
}

void ButiEngine::GunAction_BossSpin::Start()
{
}

void ButiEngine::GunAction_BossSpin::OnRemove()
{
}

void ButiEngine::GunAction_BossSpin::OnShowUI()
{
	GUI::BulletText(u8"waitTimer");
	GUI::Text(m_vlp_waitTimer->GetCurrentCountFrame());
	GUI::BulletText(u8"actionTimer");
	GUI::Text(m_vlp_actionTimer->GetCurrentCountFrame());
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::GunAction_BossSpin::Clone()
{
	return ObjectFactory::Create<GunAction_BossSpin>();
}

void ButiEngine::GunAction_BossSpin::StartPause()
{
	m_vlp_waitTimer->Stop();
	m_vlp_actionTimer->Stop();
	m_vwp_rightGunComponent.lock()->ShootStop();
	m_vwp_leftGunComponent.lock()->ShootStop();
}

void ButiEngine::GunAction_BossSpin::FinishPause()
{
	m_vlp_waitTimer->Start();
	m_vlp_actionTimer->Start();
	if (m_phase == GunAction_BossSpinPhase::Shoot)
	{
		m_vwp_rightGunComponent.lock()->ShootStart();
		m_vwp_leftGunComponent.lock()->ShootStart();
	}
}

void ButiEngine::GunAction_BossSpin::StartBeforeShootPhase()
{
	m_phase = GunAction_BossSpinPhase::BeforeShoot;
	m_vlp_waitTimer->Reset();
	m_vlp_waitTimer->Start();
}

void ButiEngine::GunAction_BossSpin::UpdateBeforeShootPhase()
{
	if (m_vlp_waitTimer->Update())
	{
		StartShootPhase();
	}
}

void ButiEngine::GunAction_BossSpin::StartShootPhase()
{
	m_phase = GunAction_BossSpinPhase::Shoot;
	m_vlp_waitTimer->Stop();
	m_vlp_actionTimer->Reset();
	m_vlp_actionTimer->Start();
	m_vwp_rightGunComponent.lock()->ShootStart();
	m_vwp_leftGunComponent.lock()->ShootStart();
}

void ButiEngine::GunAction_BossSpin::UpdateShootPhase()
{
	Rotate();

	if (m_vlp_actionTimer->Update())
	{
		m_vwp_rightGunComponent.lock()->ShootStop();
		m_vwp_leftGunComponent.lock()->ShootStop();

		StartAfterShootPhase();
	}
}

void ButiEngine::GunAction_BossSpin::StartAfterShootPhase()
{
	m_phase = GunAction_BossSpinPhase::AfterShoot;
	m_vlp_actionTimer->Stop();
	m_vlp_waitTimer->Reset();
	m_vlp_waitTimer->Start();
}

void ButiEngine::GunAction_BossSpin::UpdateAfterShootPhase()
{
	if (m_vlp_waitTimer->Update())
	{
		//I—¹ˆ—
		m_vwp_bossEnemyComponent.lock()->FinishGunAction();
		m_vwp_drawObject.lock()->GetGameComponent<LookAtComponent>()->SetIsActive(true);
		m_vwp_bossEnemyComponent.lock()->ChangeGun("Gun_Boss_Normal");
		gameObject.lock()->GetGameComponent<EquipGun>()->RemoveGun(1);
		SetIsRemove(true);
	}
}

void ButiEngine::GunAction_BossSpin::Rotate()
{
	float rollUnit = MathHelper::ToRadian(360.0f / (m_actionTime / static_cast<float>(m_spinCount)));
	m_vwp_drawObject.lock()->transform->RollLocalRotationY_Radian(rollUnit);
}
