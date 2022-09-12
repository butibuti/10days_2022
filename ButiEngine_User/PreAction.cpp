#include "stdafx_u.h"
#include "PreAction.h"
#include "Player.h"
#include "SeparateDrawObject.h"
#include "EquipGun.h"
#include "GunAction_AssaultRifle.h"
#include "GunAction_GrenadeLauncher.h"
#include "GunAction_Shotgun.h"
#include "Camera.h"
#include "Header/GameObjects/DefaultGameComponent/RotationAnimationComponent.h"

void ButiEngine::PreAction::OnUpdate()
{
	switch (m_phase)
	{
	case ButiEngine::PreActioinPhase::LookCamera:
		UpdateLookCameraPhase();
		break;
	case ButiEngine::PreActioinPhase::Wait:
		UpdateWaitPhase();
		break;
	case ButiEngine::PreActioinPhase::Return:
		UpdateReturnPhase();
		break;
	default:
		break;
	}
}

void ButiEngine::PreAction::OnSet()
{
	gameObject.lock()->GetGameComponent<Player>()->StartGunAction();

	m_vlp_lookCameraTimer = ObjectFactory::Create<RelativeTimer>(30);
	m_vlp_waitTimer = ObjectFactory::Create<RelativeTimer>(90);
	m_vlp_returnTimer = ObjectFactory::Create<RelativeTimer>(30);

	m_vwp_drawObject = gameObject.lock()->GetGameComponent<SeparateDrawObject>()->GetDrawObject();
	m_vwp_drawObject.lock()->GetGameComponent<LookAtComponent>()->SetIsActive(false);

	ChangeGun();

	m_vlp_startPlayerTransform = m_vwp_drawObject.lock()->transform->Clone();
	m_vlp_startRightGunTransform = m_vwp_rightGun.lock()->transform->Clone();
	//¶Žè‚ÉŽ‚Á‚Ä‚¢‚ée‚Í•K‚¸‚ ‚é‚í‚¯‚Å‚Í‚È‚¢‚Ì‚Å‚ ‚é‚©Šm”F‚·‚é
	if (m_vwp_leftGun.lock())
	{
		m_vlp_startLeftGunTransform = m_vwp_leftGun.lock()->transform->Clone();
	}

	StartLookCameraPhase();
}

void ButiEngine::PreAction::Start()
{
}

void ButiEngine::PreAction::OnRemove()
{
}

void ButiEngine::PreAction::OnShowUI()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::PreAction::Clone()
{
	return ObjectFactory::Create<PreAction>();
}

void ButiEngine::PreAction::StartLookCameraPhase()
{
	m_phase = PreActioinPhase::LookCamera;
	AddStartTransformAnimation_Player();
	AddStartTransformAnimation_RightGun();
	AddStartTransformAnimation_LeftGun();
	GetManager().lock()->GetGameObject("CameraMan").lock()->GetGameComponent<Camera>()->Approach(m_vlp_lookCameraTimer->GetMaxCountFrame());
	m_vlp_lookCameraTimer->Start();
}

void ButiEngine::PreAction::UpdateLookCameraPhase()
{
	if (m_vlp_lookCameraTimer->Update())
	{
		m_vlp_lookCameraTimer->Stop();
		StartWaitPhase();
	}
}

void ButiEngine::PreAction::StartWaitPhase()
{
	m_phase = PreActioinPhase::Wait;
	m_vlp_waitTimer->Start();
}

void ButiEngine::PreAction::UpdateWaitPhase()
{
	if (m_vlp_waitTimer->Update())
	{
		m_vlp_waitTimer->Stop();
		StartReturnPhase();
	}
}

void ButiEngine::PreAction::StartReturnPhase()
{
	m_phase = PreActioinPhase::Return;
	AddFinishTransformAnimation_Player();
	AddFinishTransformAnimation_RightGun();
	AddFinishTransformAnimation_LeftGun();
	GetManager().lock()->GetGameObject("CameraMan").lock()->GetGameComponent<Camera>()->ReturnDefault(m_vlp_returnTimer->GetMaxCountFrame());
	m_vlp_returnTimer->Start();
}

void ButiEngine::PreAction::UpdateReturnPhase()
{
	if (m_vlp_returnTimer->Update())
	{
		m_vlp_returnTimer->Stop();
		AddGunAction();
		SetIsRemove(true);
	}
}

void ButiEngine::PreAction::ChangeGun()
{
	auto playerComponent = gameObject.lock()->GetGameComponent<Player>();
	auto equipGunComponent = gameObject.lock()->GetGameComponent<EquipGun>();
	switch (m_gunActionType)
	{
	case ButiEngine::GunActionType::AssaultRifle:
		m_vwp_rightGun = playerComponent->ChangeGun("Gun_Player_AssaultRifle");
		break;
	case ButiEngine::GunActionType::GrenadeLauncher:
		m_vwp_rightGun = playerComponent->ChangeGun("Gun_Player_GrenadeLauncher_Right");
		m_vwp_leftGun = equipGunComponent->AddGun("Gun_Player_GrenadeLauncher_Left");
		break;
	case ButiEngine::GunActionType::Shotgun:
		m_vwp_rightGun = playerComponent->ChangeGun("Gun_Player_Shotgun");
		break;
	default:
		break;
	}
}

void ButiEngine::PreAction::AddGunAction()
{
	switch (m_gunActionType)
	{
	case ButiEngine::GunActionType::AssaultRifle:
		gameObject.lock()->AddGameComponent<GunAction_AssaultRifle>();
		break;
	case ButiEngine::GunActionType::GrenadeLauncher:
		gameObject.lock()->AddGameComponent<GunAction_GrenadeLauncher>();
		break;
	case ButiEngine::GunActionType::Shotgun:
		gameObject.lock()->AddGameComponent<GunAction_Shotgun>();
		break;
	default:
		break;
	}
}

void ButiEngine::PreAction::AddStartTransformAnimation_Player()
{
	auto target = m_vwp_drawObject.lock()->transform->Clone();
	Vector3 lookPos = gameObject.lock()->transform->GetLocalPosition() + Vector3Const::ZAxis * -100.0f;
	target->SetLookAtRotation(lookPos);

	auto anim = m_vwp_drawObject.lock()->AddGameComponent<RotationAnimation>();
	anim->SetInitRotate(m_vwp_drawObject.lock()->transform->GetLocalRotation());
	anim->SetTargetRotate(target->GetLocalRotation());
	anim->SetSpeed(1.0f / m_vlp_lookCameraTimer->GetMaxCountFrame());
	anim->SetEaseType(Easing::EasingType::EaseOutExpo);
}

void ButiEngine::PreAction::AddFinishTransformAnimation_Player()
{
	auto anim = m_vwp_drawObject.lock()->AddGameComponent<RotationAnimation>();
	anim->SetInitRotate(m_vwp_drawObject.lock()->transform->GetLocalRotation());
	anim->SetTargetRotate(m_vlp_startPlayerTransform->GetLocalRotation());
	anim->SetSpeed(1.0f / m_vlp_returnTimer->GetMaxCountFrame());
	anim->SetEaseType(Easing::EasingType::EaseOutExpo);
}

void ButiEngine::PreAction::AddStartTransformAnimation_RightGun()
{
}

void ButiEngine::PreAction::AddFinishTransformAnimation_RightGun()
{
}

void ButiEngine::PreAction::AddStartTransformAnimation_LeftGun()
{
}

void ButiEngine::PreAction::AddFinishTransformAnimation_LeftGun()
{
}
