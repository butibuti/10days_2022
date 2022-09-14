#include "stdafx_u.h"
#include "GunAction_BossSnake.h"
#include "EquipGun.h"
#include "Gun.h"
#include "Player.h"
#include "SeparateDrawObject.h"
#include "BossEnemy.h"
#include "Header/GameObjects/DefaultGameComponent/RigidBodyComponent.h"
#include "Header/GameObjects/DefaultGameComponent/PositionAnimationComponent.h"
#include "Header/GameObjects/DefaultGameComponent/RotationAnimationComponent.h"

void ButiEngine::GunAction_BossSnake::OnUpdate()
{
	if (m_vwp_bossEnemyComponent.lock()->IsPause())
	{
		return;
	}

	switch (m_phase)
	{
	case ButiEngine::GunAction_BossSnakePhase::BeforeShoot:
		UpdateBeforeShootPhase();
		break;
	case ButiEngine::GunAction_BossSnakePhase::Shoot:
		UpdateShootPhase();
		break;
	case ButiEngine::GunAction_BossSnakePhase::AfterShoot:
		UpdateAfterShootPhase();
		break;
	default:
		break;
	}
}

void ButiEngine::GunAction_BossSnake::OnSet()
{
	m_vwp_drawObject = gameObject.lock()->GetGameComponent<SeparateDrawObject>()->GetDrawObject();
	//m_vwp_drawObject.lock()->GetGameComponent<LookAtComponent>()->SetIsActive(false);
	m_vwp_player = GetManager().lock()->GetGameObject("Player");
	m_vwp_lookAt = m_vwp_drawObject.lock()->GetGameComponent<LookAtComponent>();

	m_vwp_bossEnemyComponent = gameObject.lock()->GetGameComponent<BossEnemy>();
	m_vwp_bossEnemyComponent.lock()->StartGunAction();

	m_vwp_gunComponent = m_vwp_bossEnemyComponent.lock()->ChangeGun("Gun_Boss_Snake");

	m_lockOnTime = 80;
	m_restTime = 20;
	m_actionTime = 40;
	m_vlp_waitTimer = ObjectFactory::Create<RelativeTimer>(m_lockOnTime);
	m_vlp_actionTimer = ObjectFactory::Create<RelativeTimer>(m_actionTime);
	m_turnCount = 0;
	m_turnCountMax = 6;
	m_spreadDegree = 100;

	StartBeforeShootPhase();
}

void ButiEngine::GunAction_BossSnake::Start()
{
}

void ButiEngine::GunAction_BossSnake::OnRemove()
{
}

void ButiEngine::GunAction_BossSnake::OnShowUI()
{
	GUI::BulletText(u8"waitTimer");
	GUI::Text(m_vlp_waitTimer->GetCurrentCountFrame());
	GUI::BulletText(u8"actionTimer");
	GUI::Text(m_vlp_actionTimer->GetCurrentCountFrame());
	GUI::BulletText(u8"turnCount");
	GUI::Text(m_turnCount);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::GunAction_BossSnake::Clone()
{
	return ObjectFactory::Create<GunAction_BossSnake>();
}

void ButiEngine::GunAction_BossSnake::StartPause()
{
	m_vlp_waitTimer->Stop();
	m_vlp_actionTimer->Stop();
	m_vwp_gunComponent.lock()->ShootStop();
	m_vwp_drawObject.lock()->GetGameComponent<LookAtComponent>()->SetIsActive(false);
}

void ButiEngine::GunAction_BossSnake::FinishPause()
{
	m_vlp_waitTimer->Start();
	m_vlp_actionTimer->Start();
	if (m_phase == GunAction_BossSnakePhase::Shoot)
	{
		m_vwp_gunComponent.lock()->ShootStart();
	}

	if (m_phase == GunAction_BossSnakePhase::BeforeShoot)
	{
		m_vwp_drawObject.lock()->GetGameComponent<LookAtComponent>()->SetIsActive(true);
	}
	else
	{
		m_vwp_drawObject.lock()->GetGameComponent<LookAtComponent>()->SetIsActive(false);
	}
}

void ButiEngine::GunAction_BossSnake::StartBeforeShootPhase()
{
	m_phase = GunAction_BossSnakePhase::BeforeShoot;
	m_vlp_waitTimer->Reset();
	m_vlp_waitTimer->Start();
}

void ButiEngine::GunAction_BossSnake::UpdateBeforeShootPhase()
{
	auto lookTarget = m_vwp_lookAt.lock()->GetLookTarget();
	Vector3 playerPos = m_vwp_player.lock()->transform->GetLocalPosition();
	lookTarget->SetLocalPosition(Vector3(playerPos.x, m_vwp_drawObject.lock()->transform->GetWorldPosition().y, playerPos.z));

	if (m_vlp_waitTimer->Update())
	{
		StartShootPhase();
	}
}

void ButiEngine::GunAction_BossSnake::StartShootPhase()
{
	m_phase = GunAction_BossSnakePhase::Shoot;
	m_vlp_waitTimer->Stop();
	m_vlp_actionTimer->ChangeCountFrame(m_actionTime / 2.0f);
	m_vlp_actionTimer->Reset();
	m_vlp_actionTimer->Start();
	m_vwp_gunComponent.lock()->ShootStart();
	m_vwp_drawObject.lock()->GetGameComponent<LookAtComponent>()->SetIsActive(false);
}

void ButiEngine::GunAction_BossSnake::UpdateShootPhase()
{
	Rotate();

	if (m_turnCount > m_turnCountMax)
	{
		m_vwp_gunComponent.lock()->ShootStop();

		StartAfterShootPhase();
	}
}

void ButiEngine::GunAction_BossSnake::StartAfterShootPhase()
{
	m_phase = GunAction_BossSnakePhase::AfterShoot;
	m_vlp_actionTimer->Stop();
	m_vlp_waitTimer->ChangeCountFrame(m_restTime);
	m_vlp_waitTimer->Reset();
	m_vlp_waitTimer->Start();
}

void ButiEngine::GunAction_BossSnake::UpdateAfterShootPhase()
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

void ButiEngine::GunAction_BossSnake::Rotate()
{
	float rollUnit = MathHelper::ToRadian(m_spreadDegree / static_cast<float>(m_actionTime));
	if (m_turnCount % 2 == 1)
	{
		rollUnit *= -1;
	}

	m_vwp_drawObject.lock()->transform->RollLocalRotationY_Radian(rollUnit);

	if (m_vlp_actionTimer->Update())
	{
		if (m_turnCount == 0)
		{
			m_vlp_actionTimer->ChangeCountFrame(m_actionTime);
		}
		m_vlp_actionTimer->Reset();
		m_turnCount++;
	}
}
