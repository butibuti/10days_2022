#include "stdafx_u.h"
#include "GunAction_Shotgun.h"
#include "EquipGun.h"
#include "Gun.h"
#include "SeparateDrawObject.h"
#include "Player.h"
#include "Header/GameObjects/DefaultGameComponent/RigidBodyComponent.h"
#include "Header/GameObjects/DefaultGameComponent/PositionAnimationComponent.h"

void ButiEngine::GunAction_Shotgun::OnUpdate()
{
	switch (m_phase)
	{
	case ButiEngine::GunAction_ShotgunPhase::MoveOffScreen:
		UpdateMoveOffScreenPhase();
		break;
	case ButiEngine::GunAction_ShotgunPhase::Shoot:
		UpdateShootPhase();
		break;
	case ButiEngine::GunAction_ShotgunPhase::ReturnCenter:
		UpdateReturnCenterPhase();
		break;
	default:
		break;
	}
}

void ButiEngine::GunAction_Shotgun::OnSet()
{
	GetManager().lock()->GetGameObject("CameraMan").lock()->transform->SetBaseTransform(nullptr);

	m_vwp_drawObject = gameObject.lock()->GetGameComponent<SeparateDrawObject>()->GetDrawObject();
	m_vwp_drawObject.lock()->GetGameComponent<LookAtComponent>()->SetIsActive(false);

	m_vwp_playerComponent = gameObject.lock()->GetGameComponent<Player>();
	m_vwp_playerComponent.lock()->StartGunAction();

	auto rigidBodyComponent = gameObject.lock()->GetGameComponent<RigidBodyComponent>();
	rigidBodyComponent->GetRigidBody()->SetVelocity(Vector3Const::Zero);
	rigidBodyComponent->UnRegist();

	m_vwp_gunComponent = m_vwp_playerComponent.lock()->ChangeGun("Gun_Player_Shotgun");


	std::int32_t moveOffScreenPhaseFrame = 20;
	m_vlp_moveOffScreenPhaseTimer = ObjectFactory::Create<RelativeTimer>(moveOffScreenPhaseFrame);

	std::int32_t shootPhaseFrame = 60;
	m_vlp_shootPhaseTimer = ObjectFactory::Create<RelativeTimer>(shootPhaseFrame);

	std::int32_t returnCenterPhaseFrame = 20;
	m_vlp_returnCenterPhaseTimer = ObjectFactory::Create<RelativeTimer>(returnCenterPhaseFrame);

	m_vwp_startTransform = gameObject.lock()->transform->Clone();
	m_vwp_startTransform->SetLocalRotation(m_vwp_drawObject.lock()->transform->GetLocalRotation());

	StartMoveOffScreenPhase();
}

void ButiEngine::GunAction_Shotgun::Start()
{
}

void ButiEngine::GunAction_Shotgun::OnRemove()
{
}

void ButiEngine::GunAction_Shotgun::OnShowUI()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::GunAction_Shotgun::Clone()
{
	return ObjectFactory::Create<GunAction_Shotgun>();
}

void ButiEngine::GunAction_Shotgun::StartMoveOffScreenPhase()
{
	m_phase = GunAction_ShotgunPhase::MoveOffScreen;

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

void ButiEngine::GunAction_Shotgun::UpdateMoveOffScreenPhase()
{
	if (m_vlp_moveOffScreenPhaseTimer->Update())
	{
		m_vlp_moveOffScreenPhaseTimer->Stop();
		StartShootPhase();
	}
}

void ButiEngine::GunAction_Shotgun::StartShootPhase()
{
	m_phase = GunAction_ShotgunPhase::Shoot;

	m_vwp_drawObject.lock()->transform->SetLocalRotationY_Degrees(90.0f);

	auto anim = gameObject.lock()->AddGameComponent<PositionAnimation>();

	anim->SetInitPosition(gameObject.lock()->transform->GetLocalPosition());
	Vector3 targetPos = gameObject.lock()->transform->GetLocalPosition();
	targetPos.x += 50.0f;
	anim->SetTargetPosition(targetPos);
	anim->SetSpeed(1.0f / m_vlp_shootPhaseTimer->GetMaxCountFrame());
	anim->SetEaseType(Easing::EasingType::Liner);

	m_vwp_gunComponent.lock()->ShootStart();

	m_vlp_shootPhaseTimer->Start();
}

void ButiEngine::GunAction_Shotgun::UpdateShootPhase()
{
	if (m_vlp_shootPhaseTimer->Update())
	{
		m_vlp_shootPhaseTimer->Stop();
		m_vwp_gunComponent.lock()->ShootStop();
		StartReturnCenterPhase();
	}
}

void ButiEngine::GunAction_Shotgun::StartReturnCenterPhase()
{
	m_phase = GunAction_ShotgunPhase::ReturnCenter;

	m_vwp_drawObject.lock()->transform->SetLocalRotation(m_vwp_startTransform->GetLocalRotation());
	
	auto anim = gameObject.lock()->AddGameComponent<PositionAnimation>();
	Vector3 initPos = m_vwp_startTransform->GetLocalPosition();
	initPos.z += 20.0f;
	anim->SetInitPosition(initPos);
	anim->SetTargetPosition(m_vwp_startTransform->GetLocalPosition());
	anim->SetSpeed(1.0f / m_vlp_returnCenterPhaseTimer->GetMaxCountFrame());
	anim->SetEaseType(Easing::EasingType::EaseOutExpo);

	m_vwp_playerComponent.lock()->ChangeGun("Gun_Player_Normal");

	m_vlp_returnCenterPhaseTimer->Start();
}

void ButiEngine::GunAction_Shotgun::UpdateReturnCenterPhase()
{
	if (m_vlp_returnCenterPhaseTimer->Update())
	{
		m_vwp_playerComponent.lock()->FinishGunAction();
		GetManager().lock()->GetGameObject("CameraMan").lock()->transform->SetBaseTransform(gameObject.lock()->transform);
		m_vwp_drawObject.lock()->GetGameComponent<LookAtComponent>()->SetIsActive(true);
		gameObject.lock()->GetGameComponent<RigidBodyComponent>()->Regist();
		SetIsRemove(true);
		return;
	}
}
