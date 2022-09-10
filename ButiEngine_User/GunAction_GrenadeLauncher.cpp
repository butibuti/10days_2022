#include "stdafx_u.h"
#include "GunAction_GrenadeLauncher.h"
#include "SeparateDrawObject.h"
#include "Player.h"
#include "Gun.h"
#include "Header/GameObjects/DefaultGameComponent/RigidBodyComponent.h"

void ButiEngine::GunAction_GrenadeLauncher::OnUpdate()
{
	switch (m_phase)
	{
	case ButiEngine::GunAction_GrenadeLauncherPhase::Shoot:
		UpdateShootPhase();
		break;
	case ButiEngine::GunAction_GrenadeLauncherPhase::Wait:
		UpdateWaitPhase();
		break;
	default:
		break;
	}
}

void ButiEngine::GunAction_GrenadeLauncher::OnSet()
{
	gameObject.lock()->GetGameComponent<RigidBodyComponent>()->GetRigidBody()->SetVelocity(Vector3Const::Zero);

	m_vwp_playerComponent = gameObject.lock()->GetGameComponent<Player>();
	m_vwp_playerComponent.lock()->StartGunAction();

	m_vwp_drawObject = gameObject.lock()->GetGameComponent<SeparateDrawObject>()->GetDrawObject();
	m_vwp_lookTarget = m_vwp_drawObject.lock()->GetGameComponent<LookAtComponent>()->GetLookTarget();

	m_vwp_gunComponent = m_vwp_playerComponent.lock()->ChangeGun("Gun_Player_GrenadeLauncher");


	std::int32_t shootPhaseFrame = 18;
	m_vlp_shootTimer = ObjectFactory::Create<RelativeTimer>(shootPhaseFrame);
	m_rotationAngle = 360.0f / shootPhaseFrame;

	std::int32_t waitPhaseFrame = 60;
	m_vlp_waitTimer = ObjectFactory::Create<RelativeTimer>(waitPhaseFrame);

	StartShootPhase();
}

void ButiEngine::GunAction_GrenadeLauncher::Start()
{
}

void ButiEngine::GunAction_GrenadeLauncher::OnRemove()
{
}

void ButiEngine::GunAction_GrenadeLauncher::OnShowUI()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::GunAction_GrenadeLauncher::Clone()
{
	return ObjectFactory::Create<GunAction_GrenadeLauncher>();
}

void ButiEngine::GunAction_GrenadeLauncher::StartShootPhase()
{
	m_phase = GunAction_GrenadeLauncherPhase::Shoot;
	m_vlp_shootTimer->Start();
	m_vwp_gunComponent.lock()->ShootStart();
}

void ButiEngine::GunAction_GrenadeLauncher::UpdateShootPhase()
{
	if (m_vlp_shootTimer->Update())
	{
		m_vlp_shootTimer->Stop();
		m_vwp_gunComponent.lock()->ShootStop();

		StartWaitPhase();
	}

	auto drawObjectTransform = m_vwp_drawObject.lock()->transform;
	drawObjectTransform->RollWorldRotationY_Degrees(m_rotationAngle);
	m_vwp_lookTarget.lock()->SetLocalPosition(drawObjectTransform->GetWorldPosition() + drawObjectTransform->GetFront());
}

void ButiEngine::GunAction_GrenadeLauncher::StartWaitPhase()
{
	m_phase = GunAction_GrenadeLauncherPhase::Wait;
	m_vlp_waitTimer->Start();
}

void ButiEngine::GunAction_GrenadeLauncher::UpdateWaitPhase()
{
	if (m_vlp_waitTimer->Update())
	{
		m_vwp_playerComponent.lock()->FinishGunAction();
		m_vwp_playerComponent.lock()->ChangeGun("Gun_Player_Normal");
		SetIsRemove(true);
		return;
	}
}
