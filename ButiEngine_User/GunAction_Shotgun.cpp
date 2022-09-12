#include "stdafx_u.h"
#include "GunAction_Shotgun.h"
#include "EquipGun.h"
#include "Gun.h"
#include "SeparateDrawObject.h"
#include "Player.h"
#include "Header/GameObjects/DefaultGameComponent/PositionAnimationComponent.h"
#include "Header/GameObjects/DefaultGameComponent/RotationAnimationComponent.h"

void ButiEngine::GunAction_Shotgun::OnUpdate()
{
	switch (m_phase)
	{
	case ButiEngine::GunAction_ShotgunPhase::Move:
		UpdateMovePhase();
		break;
	case ButiEngine::GunAction_ShotgunPhase::MoveWait:
		UpdateMoveWaitPhase();
		break;
	case ButiEngine::GunAction_ShotgunPhase::Shoot:
		UpdateShootPhase();
		break;
	case ButiEngine::GunAction_ShotgunPhase::Return:
		UpdateReturnPhase();
		break;
	default:
		break;
	}
}

void ButiEngine::GunAction_Shotgun::OnSet()
{
	m_vwp_drawObject = gameObject.lock()->GetGameComponent<SeparateDrawObject>()->GetDrawObject();
	m_vwp_drawObject.lock()->GetGameComponent<LookAtComponent>()->SetIsActive(true);

	m_vwp_playerComponent = gameObject.lock()->GetGameComponent<Player>();

	m_vwp_gunComponent = gameObject.lock()->GetGameComponent<EquipGun>()->GetGun(0).lock()->GetGameComponent<Gun>();

	std::int32_t movePhaseFrame = 20;
	m_vlp_movePhaseTimer = ObjectFactory::Create<RelativeTimer>(movePhaseFrame);

	std::int32_t moveWaitPhaseFrame = 20;
	m_vlp_moveWaitPhaseTimer = ObjectFactory::Create<RelativeTimer>(moveWaitPhaseFrame);

	std::int32_t shootPhaseFrame = 90;
	m_vlp_shootPhaseTimer = ObjectFactory::Create<RelativeTimer>(shootPhaseFrame);
	m_maxShootCount = 5;
	m_shootCount = 0;
	m_vlp_moveShootTimer = ObjectFactory::Create<RelativeTimer>(shootPhaseFrame / m_maxShootCount);
	m_vlp_moveShootCenterTransform = gameObject.lock()->transform->Clone();
	m_radius = 5.0f;

	std::int32_t shootWaitPhaseFrame = 30;
	m_vlp_returnTimer = ObjectFactory::Create<RelativeTimer>(shootWaitPhaseFrame);

	StartMovePhase();
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

void ButiEngine::GunAction_Shotgun::StartMovePhase()
{
	m_phase = GunAction_ShotgunPhase::Move;

	auto anim = gameObject.lock()->AddGameComponent<PositionAnimation>();
	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();
	anim->SetInitPosition(pos);
	Vector3 targetPos = pos;
	targetPos.x -= m_radius;
	anim->SetTargetPosition(targetPos);
	anim->SetSpeed(1.0f / m_vlp_movePhaseTimer->GetMaxCountFrame());
	anim->SetEaseType(Easing::EasingType::EaseOutCirc);

	auto lookAt = m_vwp_drawObject.lock()->GetGameComponent<LookAtComponent>();
	lookAt->GetLookTarget()->SetLocalPosition(pos);
	lookAt->GetLookTarget()->TranslateX(1.0f);
	lookAt->SetSpeed(0.6f);


	m_vlp_movePhaseTimer->Start();
}

void ButiEngine::GunAction_Shotgun::UpdateMovePhase()
{
	if (m_vlp_movePhaseTimer->Update())
	{
		m_vlp_movePhaseTimer->Stop();
		m_vwp_drawObject.lock()->GetGameComponent<LookAtComponent>()->GetLookTarget()->SetLocalPosition(m_vlp_moveShootCenterTransform->GetLocalPosition());
		StartMoveWaitPhase();
	}
}

void ButiEngine::GunAction_Shotgun::StartMoveWaitPhase()
{
	m_phase = GunAction_ShotgunPhase::MoveWait;
	m_vlp_moveWaitPhaseTimer->Start();
}

void ButiEngine::GunAction_Shotgun::UpdateMoveWaitPhase()
{
	if (m_vlp_moveWaitPhaseTimer->Update())
	{
		m_vlp_moveWaitPhaseTimer->Stop();
		StartShootPhase();
	}
}

void ButiEngine::GunAction_Shotgun::StartShootPhase()
{
	m_phase = GunAction_ShotgunPhase::Shoot;
	m_vwp_gunComponent.lock()->ShootStart();

	m_vlp_moveShootCenterTransform->SetLookAtRotation(gameObject.lock()->transform->GetLocalPosition());
	AddShootAnimation();

	m_vlp_moveShootTimer->Start();
	m_vlp_shootPhaseTimer->Start();
}

void ButiEngine::GunAction_Shotgun::UpdateShootPhase()
{
	if (m_vlp_shootPhaseTimer->Update())
	{
		m_vlp_shootPhaseTimer->Stop();
		m_vwp_gunComponent.lock()->ShootStop();
		StartReturnPhase();
		return;
	}

	if (m_vlp_moveShootTimer->Update())
	{
		AddShootAnimation();
	}
}

void ButiEngine::GunAction_Shotgun::AddShootAnimation()
{
	m_shootCount++;

	m_vlp_moveShootCenterTransform->RollLocalRotationY_Degrees(360.0f / m_maxShootCount);

	auto anim = gameObject.lock()->AddGameComponent<PositionAnimation>();
	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();
	anim->SetInitPosition(pos);
	Vector3 targetPos = m_vlp_moveShootCenterTransform->GetLocalPosition() + m_vlp_moveShootCenterTransform->GetFront() * m_radius;
	if (m_shootCount >= m_maxShootCount)
	{
		targetPos = m_vlp_moveShootCenterTransform->GetLocalPosition();
		auto lookAt = m_vwp_drawObject.lock()->GetGameComponent<LookAtComponent>();
		lookAt->SetSpeed(0.3f);
		lookAt->GetLookTarget()->TranslateX(1.0f);
	}
	anim->SetTargetPosition(targetPos);
	anim->SetSpeed(1.0f / m_vlp_moveShootTimer->GetMaxCountFrame());
	anim->SetEaseType(Easing::EasingType::EaseOutCirc);
}

void ButiEngine::GunAction_Shotgun::StartReturnPhase()
{
	m_phase = GunAction_ShotgunPhase::Return;

	auto anim = gameObject.lock()->AddGameComponent<PositionAnimation>();
	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();
	anim->SetInitPosition(pos);
	Vector3 targetPos = m_vlp_moveShootCenterTransform->GetLocalPosition();
	anim->SetTargetPosition(targetPos);
	anim->SetSpeed(1.0f / m_vlp_returnTimer->GetMaxCountFrame());
	anim->SetEaseType(Easing::EasingType::EaseOutCirc);

	m_vlp_returnTimer->Start();
}

void ButiEngine::GunAction_Shotgun::UpdateReturnPhase()
{
	if (m_vlp_returnTimer->Update())
	{
		m_vlp_returnTimer->Stop();
		m_vwp_playerComponent.lock()->FinishGunAction();
		m_vwp_playerComponent.lock()->ChangeGun("Gun_Player_Normal");
		m_vwp_drawObject.lock()->GetGameComponent<LookAtComponent>()->SetSpeed(0.1f);
		SetIsRemove(true);
	}
}
