#include "stdafx_u.h"
#include "GunAction_GrenadeLauncher.h"
#include "EquipGun.h"
#include "Gun.h"
#include "SeparateDrawObject.h"
#include "Player.h"
#include "Header/GameObjects/DefaultGameComponent/RigidBodyComponent.h"
#include "Header/GameObjects/DefaultGameComponent/PositionAnimationComponent.h"

void ButiEngine::GunAction_GrenadeLauncher::OnUpdate()
{
	switch (m_phase)
	{
	case ButiEngine::GunAction_GrenadeLauncherPhase::MoveOffScreen:
		UpdateMoveOffScreenPhase();
		break;
	case ButiEngine::GunAction_GrenadeLauncherPhase::MoveOffScreenWait:
		UpdateMoveOffScreenWaitPhase();
		break;
	case ButiEngine::GunAction_GrenadeLauncherPhase::Shoot:
		UpdateShootPhase();
		break;
	case ButiEngine::GunAction_GrenadeLauncherPhase::ShootWait:
		UpdateShootWaitPhase();
		break;
	case ButiEngine::GunAction_GrenadeLauncherPhase::ReturnCenter:
		UpdateReturnCenterPhase();
		break;
	default:
		break;
	}
}

void ButiEngine::GunAction_GrenadeLauncher::OnSet()
{
	GetManager().lock()->GetGameObject("CameraMan").lock()->transform->SetBaseTransform(nullptr);

	m_vwp_drawObject = gameObject.lock()->GetGameComponent<SeparateDrawObject>()->GetDrawObject();
	m_vwp_drawObject.lock()->GetGameComponent<LookAtComponent>()->SetIsActive(false);

	m_vwp_playerComponent = gameObject.lock()->GetGameComponent<Player>();
	m_vwp_playerComponent.lock()->StartGunAction();

	auto rigidBodyComponent = gameObject.lock()->GetGameComponent<RigidBodyComponent>();
	rigidBodyComponent->GetRigidBody()->SetVelocity(Vector3Const::Zero);
	rigidBodyComponent->UnRegist();

	m_vwp_rightGunComponent = m_vwp_playerComponent.lock()->ChangeGun("Gun_Player_GrenadeLauncher_Right");
	m_vwp_leftGunComponent = gameObject.lock()->GetGameComponent<EquipGun>()->AddGun("Gun_Player_GrenadeLauncher_Left").lock()->GetGameComponent<Gun>();


	std::int32_t moveOffScreenPhaseFrame = 20;
	m_vlp_moveOffScreenPhaseTimer = ObjectFactory::Create<RelativeTimer>(moveOffScreenPhaseFrame);

	std::int32_t moveOffScreenWaitPhaseFrame = 30;
	m_vlp_moveOffScreenWaitPhaseTimer = ObjectFactory::Create<RelativeTimer>(moveOffScreenWaitPhaseFrame);

	std::int32_t shootPhaseFrame = 60;
	m_vlp_shootPhaseTimer = ObjectFactory::Create<RelativeTimer>(shootPhaseFrame);

	std::int32_t shootWaitPhaseFrame = 90;
	m_vlp_shootWaitPhaseTimer = ObjectFactory::Create<RelativeTimer>(shootWaitPhaseFrame);

	std::int32_t returnCenterPhaseFrame = 20;
	m_vlp_returnCenterPhaseTimer = ObjectFactory::Create<RelativeTimer>(returnCenterPhaseFrame);

	m_vwp_startTransform = gameObject.lock()->transform->Clone();
	m_vwp_startTransform->SetLocalRotation(m_vwp_drawObject.lock()->transform->GetLocalRotation());

	StartMoveOffScreenPhase();
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

void ButiEngine::GunAction_GrenadeLauncher::StartMoveOffScreenPhase()
{
	m_phase = GunAction_GrenadeLauncherPhase::MoveOffScreen;

	m_vwp_drawObject.lock()->transform->SetLocalRotationY_Degrees(-90.0f);

	auto anim = gameObject.lock()->AddGameComponent<PositionAnimation>();

	anim->SetInitPosition(m_vwp_startTransform->GetLocalPosition());
	Vector3 targetPos = gameObject.lock()->transform->GetLocalPosition();
	targetPos.x -= 25.0f;
	anim->SetTargetPosition(targetPos);
	anim->SetSpeed(1.0f / m_vlp_moveOffScreenPhaseTimer->GetMaxCountFrame());
	anim->SetEaseType(Easing::EasingType::EaseOutCirc);

	m_vlp_moveOffScreenPhaseTimer->Start();
}

void ButiEngine::GunAction_GrenadeLauncher::UpdateMoveOffScreenPhase()
{
	if (m_vlp_moveOffScreenPhaseTimer->Update())
	{
		m_vlp_moveOffScreenPhaseTimer->Stop();
		StartMoveOffScreenWaitPhase();
	}
}

void ButiEngine::GunAction_GrenadeLauncher::StartMoveOffScreenWaitPhase()
{
	m_phase = GunAction_GrenadeLauncherPhase::MoveOffScreenWait;
	m_vlp_moveOffScreenWaitPhaseTimer->Start();
}

void ButiEngine::GunAction_GrenadeLauncher::UpdateMoveOffScreenWaitPhase()
{
	if (m_vlp_moveOffScreenWaitPhaseTimer->Update())
	{
		m_vlp_moveOffScreenWaitPhaseTimer->Stop();
		StartShootPhase();
	}
}

void ButiEngine::GunAction_GrenadeLauncher::StartShootPhase()
{
	m_phase = GunAction_GrenadeLauncherPhase::Shoot;

	m_vwp_drawObject.lock()->transform->SetLocalRotationY_Degrees(90.0f);

	auto anim = gameObject.lock()->AddGameComponent<PositionAnimation>();

	anim->SetInitPosition(gameObject.lock()->transform->GetLocalPosition());
	Vector3 targetPos = gameObject.lock()->transform->GetLocalPosition();
	targetPos.x += 50.0f;
	anim->SetTargetPosition(targetPos);
	anim->SetSpeed(1.0f / m_vlp_shootPhaseTimer->GetMaxCountFrame());
	anim->SetEaseType(Easing::EasingType::Liner);

	m_vwp_rightGunComponent.lock()->ShootStart();

	m_vlp_shootPhaseTimer->Start();
}

void ButiEngine::GunAction_GrenadeLauncher::UpdateShootPhase()
{
	if (m_vlp_shootPhaseTimer->GetCurrentCountFrame() == m_vwp_leftGunComponent.lock()->GetShootIntervalFrame() * 0.5f)
	{
		m_vwp_leftGunComponent.lock()->ShootStart();
	}
	if (m_vlp_shootPhaseTimer->Update())
	{
		m_vlp_shootPhaseTimer->Stop();
		m_vwp_rightGunComponent.lock()->ShootStop();
		m_vwp_leftGunComponent.lock()->ShootStop();

		StartShootWaitPhase();
	}
}

void ButiEngine::GunAction_GrenadeLauncher::StartShootWaitPhase()
{
	m_phase = GunAction_GrenadeLauncherPhase::ShootWait;
	m_vlp_shootWaitPhaseTimer->Start();
}

void ButiEngine::GunAction_GrenadeLauncher::UpdateShootWaitPhase()
{
	if (m_vlp_shootWaitPhaseTimer->Update())
	{
		m_vlp_shootWaitPhaseTimer->Stop();
		StartReturnCenterPhase();
	}
}

void ButiEngine::GunAction_GrenadeLauncher::StartReturnCenterPhase()
{
	m_phase = GunAction_GrenadeLauncherPhase::ReturnCenter;

	m_vwp_drawObject.lock()->transform->SetLocalRotation(m_vwp_startTransform->GetLocalRotation());

	auto anim = gameObject.lock()->AddGameComponent<PositionAnimation>();
	Vector3 initPos = m_vwp_startTransform->GetLocalPosition();
	initPos.z += 20.0f;
	anim->SetInitPosition(initPos);
	anim->SetTargetPosition(m_vwp_startTransform->GetLocalPosition());
	anim->SetSpeed(1.0f / m_vlp_returnCenterPhaseTimer->GetMaxCountFrame());
	anim->SetEaseType(Easing::EasingType::EaseOutExpo);

	m_vwp_playerComponent.lock()->ChangeGun("Gun_Player_Normal");
	gameObject.lock()->GetGameComponent<EquipGun>()->RemoveGun(1);

	m_vlp_returnCenterPhaseTimer->Start();
}

void ButiEngine::GunAction_GrenadeLauncher::UpdateReturnCenterPhase()
{
	if (m_vlp_returnCenterPhaseTimer->Update())
	{
		m_vlp_returnCenterPhaseTimer->Stop();
		m_vwp_playerComponent.lock()->FinishGunAction();
		GetManager().lock()->GetGameObject("CameraMan").lock()->transform->SetBaseTransform(gameObject.lock()->transform);
		m_vwp_drawObject.lock()->GetGameComponent<LookAtComponent>()->SetIsActive(true);
		gameObject.lock()->GetGameComponent<RigidBodyComponent>()->Regist();
		SetIsRemove(true);
		return;
	}
}
