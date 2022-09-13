#include "stdafx_u.h"
#include "GunAction_LastAttack.h"
#include "EquipGun.h"
#include "Gun.h"
#include "SeparateDrawObject.h"
#include "Player.h"
#include "Header/GameObjects/DefaultGameComponent/PositionAnimationComponent.h"
#include "Header/GameObjects/DefaultGameComponent/RotationAnimationComponent.h"

void ButiEngine::GunAction_LastAttack::OnUpdate()
{
	switch (m_phase)
	{
	case ButiEngine::GunAction_LastAttackPhase::AR_FirstMove:
		UpdateAR_FirstMovePhase();
		break;
	case ButiEngine::GunAction_LastAttackPhase::AR_FirstShoot:
		UpdateAR_FirstShootPhase();
		break;
	case ButiEngine::GunAction_LastAttackPhase::AR_SecondMove:
		UpdateAR_SecondMovePhase();
		break;
	case ButiEngine::GunAction_LastAttackPhase::AR_SecondShoot:
		UpdateAR_SecondShootPhase();
		break;
	case ButiEngine::GunAction_LastAttackPhase::AR_ThirdMove:
		UpdateAR_ThirdMovePhase();
		break;
	case ButiEngine::GunAction_LastAttackPhase::AR_ThirdShoot:
		UpdateAR_ThirdShootPhase();
		break;
	case ButiEngine::GunAction_LastAttackPhase::AR_SG_Wait:
		UpdateAR_SG_WaitPhase();
		break;
	case ButiEngine::GunAction_LastAttackPhase::SG_Shoot:
		UpdateSG_ShootPhase();
		break;
	case ButiEngine::GunAction_LastAttackPhase::SG_GL_Wait:
		UpdateSG_GLWaitPhase();
		break;
	case ButiEngine::GunAction_LastAttackPhase::GL_MoveOffScreen:
		UpdateGL_MoveOffScreenPhase();
		break;
	case ButiEngine::GunAction_LastAttackPhase::GL_MoveOffScreenWait:
		UpdateGL_MoveOffScreenWaitPhase();
		break;
	case ButiEngine::GunAction_LastAttackPhase::GL_FirstShoot:
		UpdateGL_FirstShootPhase();
		break;
	case ButiEngine::GunAction_LastAttackPhase::GL_SecondShoot:
		UpdateGL_SecondShootPhase();
		break;
	case ButiEngine::GunAction_LastAttackPhase::GL_ShootWait:
		UpdateGL_ShootWaitPhase();
		break;
	case ButiEngine::GunAction_LastAttackPhase::GL_ReturnCenter:
		UpdateGL_ReturnCenterPhase();
		break;
	default:
		break;
	}
}

void ButiEngine::GunAction_LastAttack::OnSet()
{
	m_vwp_drawObject = gameObject.lock()->GetGameComponent<SeparateDrawObject>()->GetDrawObject();
	auto lookAt = m_vwp_drawObject.lock()->GetGameComponent<LookAtComponent>();
	lookAt->SetIsActive(true);
	lookAt->SetSpeed(0.3f);
	

	auto equipGunComponent = gameObject.lock()->GetGameComponent<EquipGun>();
	m_vwp_rightGunComponent = equipGunComponent->GetGun(0).lock()->GetGameComponent<Gun>();
	m_vwp_leftGunComponent = equipGunComponent->GetGun(1).lock()->GetGameComponent<Gun>();

	m_vwp_startTransform = gameObject.lock()->transform->Clone();
	m_vwp_startTransform->SetLocalRotation(m_vwp_drawObject.lock()->transform->GetLocalRotation());

	m_vwp_boss = GetManager().lock()->GetGameObject("BossEnemy");

	//  AR_FirstMove,
	//	AR_FirstShoot,
	//	AR_SecondMove,
	//	AR_SecondShoot,
	//	AR_ThirdMove,
	//	AR_ThirdShoot,
	//	AR_SG_Wait,
	//	SG_Shoot,
	//	SG_GL_Wait,
	//	GL_MoveOffScreen,
	//	GL_MoveOffScreenWait,
	//	GL_FirstShoot,
	//	GL_SecondShoot,
	//	GL_ShootWait,
	//	GL_ReturnCenter,

	m_vlp_AR_firstMovePhaseTimer = ObjectFactory::Create<RelativeTimer>(20);

	m_vlp_AR_firstShootPhaseTimer = ObjectFactory::Create<RelativeTimer>(30);

	m_vlp_AR_secondMovePhaseTimer = ObjectFactory::Create<RelativeTimer>(20);

	m_vlp_AR_secondShootPhaseTimer = ObjectFactory::Create<RelativeTimer>(30);

	m_vlp_AR_thirdMovePhaseTimer = ObjectFactory::Create<RelativeTimer>(20);

	m_vlp_AR_thirdShootPhaseTimer = ObjectFactory::Create<RelativeTimer>(60);

	m_vlp_AR_SG_waitPhaseTimer = ObjectFactory::Create<RelativeTimer>(20);

	m_vlp_SG_shootPhaseTimer = ObjectFactory::Create<RelativeTimer>(180);
	m_maxShootCount = 10;
	m_shootCount = 0;
	m_vlp_moveShootTimer = ObjectFactory::Create<RelativeTimer>(m_vlp_SG_shootPhaseTimer->GetMaxCountFrame() / m_maxShootCount);
	m_radius = 7.5f;

	m_vlp_SG_GL_waitPhaseTimer = ObjectFactory::Create<RelativeTimer>(20);

	StartAR_FirstMovePhase();
}

void ButiEngine::GunAction_LastAttack::Start()
{
}

void ButiEngine::GunAction_LastAttack::OnRemove()
{
}

void ButiEngine::GunAction_LastAttack::OnShowUI()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::GunAction_LastAttack::Clone()
{
	return ObjectFactory::Create<GunAction_LastAttack>();
}

void ButiEngine::GunAction_LastAttack::StartAR_FirstMovePhase()
{
	m_phase = GunAction_LastAttackPhase::AR_FirstMove;
	m_vwp_drawObject.lock()->GetGameComponent<LookAtComponent>()->GetLookTarget()->SetLocalPosition(m_vwp_boss.lock()->transform->GetLocalPosition());
	Vector3 targetPos = m_vwp_startTransform->GetLocalPosition();
	targetPos.x += 5.0f;
	targetPos.z += 2.5f;
	AddPositionAnimation(targetPos, 1.0f / m_vlp_AR_firstMovePhaseTimer->GetMaxCountFrame(), Easing::EasingType::EaseOutCirc);
	m_vlp_AR_firstMovePhaseTimer->Start();
}

void ButiEngine::GunAction_LastAttack::UpdateAR_FirstMovePhase()
{
	if (m_vlp_AR_firstMovePhaseTimer->Update())
	{
		m_vlp_AR_firstMovePhaseTimer->Stop();
		StartAR_FirstShootPhase();
	}
}

void ButiEngine::GunAction_LastAttack::StartAR_FirstShootPhase()
{
	m_phase = GunAction_LastAttackPhase::AR_FirstShoot;
	m_vwp_rightGunComponent.lock()->ShootStart();
	m_vwp_leftGunComponent.lock()->ShootStart();
	m_vlp_AR_firstShootPhaseTimer->Start();
}

void ButiEngine::GunAction_LastAttack::UpdateAR_FirstShootPhase()
{
	if (m_vlp_AR_firstShootPhaseTimer->Update())
	{
		m_vlp_AR_firstShootPhaseTimer->Stop();
		m_vwp_rightGunComponent.lock()->ShootStop();
		m_vwp_leftGunComponent.lock()->ShootStop();
		StartAR_SecondMovePhase();
	}
}

void ButiEngine::GunAction_LastAttack::StartAR_SecondMovePhase()
{
	m_phase = GunAction_LastAttackPhase::AR_SecondMove;
	Vector3 targetPos = m_vwp_startTransform->GetLocalPosition();
	targetPos.x += -5.0f;
	targetPos.z += 2.5f;
	AddPositionAnimation(targetPos, 1.0f / m_vlp_AR_secondMovePhaseTimer->GetMaxCountFrame(), Easing::EasingType::EaseOutCirc);
	m_vlp_AR_secondMovePhaseTimer->Start();
}

void ButiEngine::GunAction_LastAttack::UpdateAR_SecondMovePhase()
{
	if (m_vlp_AR_secondMovePhaseTimer->Update())
	{
		m_vlp_AR_secondMovePhaseTimer->Stop();
		StartAR_SecondShootPhase();
	}
}

void ButiEngine::GunAction_LastAttack::StartAR_SecondShootPhase()
{
	m_phase = GunAction_LastAttackPhase::AR_SecondShoot;
	m_vwp_rightGunComponent.lock()->ShootStart();
	m_vwp_leftGunComponent.lock()->ShootStart();
	m_vlp_AR_secondShootPhaseTimer->Start();
}

void ButiEngine::GunAction_LastAttack::UpdateAR_SecondShootPhase()
{
	if (m_vlp_AR_secondShootPhaseTimer->Update())
	{
		m_vlp_AR_secondShootPhaseTimer->Stop();
		m_vwp_rightGunComponent.lock()->ShootStop();
		m_vwp_leftGunComponent.lock()->ShootStop();
		StartAR_ThirdMovePhase();
	}
}

void ButiEngine::GunAction_LastAttack::StartAR_ThirdMovePhase()
{
	m_phase = GunAction_LastAttackPhase::AR_ThirdMove;
	Vector3 targetPos = m_vwp_startTransform->GetLocalPosition();
	AddPositionAnimation(targetPos, 1.0f / m_vlp_AR_thirdMovePhaseTimer->GetMaxCountFrame(), Easing::EasingType::EaseOutCirc);
	m_vlp_AR_thirdMovePhaseTimer->Start();
}

void ButiEngine::GunAction_LastAttack::UpdateAR_ThirdMovePhase()
{
	if (m_vlp_AR_thirdMovePhaseTimer->Update())
	{
		m_vlp_AR_thirdMovePhaseTimer->Stop();
		StartAR_ThirdShootPhase();
	}
}

void ButiEngine::GunAction_LastAttack::StartAR_ThirdShootPhase()
{
	m_phase = GunAction_LastAttackPhase::AR_ThirdShoot;
	m_vwp_rightGunComponent.lock()->ShootStart();
	m_vwp_leftGunComponent.lock()->ShootStart();
	m_vlp_AR_thirdShootPhaseTimer->Start();
}

void ButiEngine::GunAction_LastAttack::UpdateAR_ThirdShootPhase()
{
	if (m_vlp_AR_thirdShootPhaseTimer->Update())
	{
		m_vlp_AR_thirdShootPhaseTimer->Stop();
		m_vwp_rightGunComponent.lock()->ShootStop();
		m_vwp_leftGunComponent.lock()->ShootStop();
		StartAR_SG_WaitPhase();
	}
}

void ButiEngine::GunAction_LastAttack::StartAR_SG_WaitPhase()
{
}

void ButiEngine::GunAction_LastAttack::UpdateAR_SG_WaitPhase()
{
}

void ButiEngine::GunAction_LastAttack::StartSG_ShootPhase()
{
}

void ButiEngine::GunAction_LastAttack::UpdateSG_ShootPhase()
{
}

void ButiEngine::GunAction_LastAttack::StartSG_GLWaitPhase()
{
}

void ButiEngine::GunAction_LastAttack::UpdateSG_GLWaitPhase()
{
}

void ButiEngine::GunAction_LastAttack::StartGL_MoveOffScreenPhase()
{
}

void ButiEngine::GunAction_LastAttack::UpdateGL_MoveOffScreenPhase()
{
}

void ButiEngine::GunAction_LastAttack::StartGL_MoveOffScreenWaitPhase()
{
}

void ButiEngine::GunAction_LastAttack::UpdateGL_MoveOffScreenWaitPhase()
{
}

void ButiEngine::GunAction_LastAttack::StartGL_FirstShootPhase()
{
}

void ButiEngine::GunAction_LastAttack::UpdateGL_FirstShootPhase()
{
}

void ButiEngine::GunAction_LastAttack::StartGL_SecondShootPhase()
{
}

void ButiEngine::GunAction_LastAttack::UpdateGL_SecondShootPhase()
{
}

void ButiEngine::GunAction_LastAttack::StartGL_ShootWaitPhase()
{
}

void ButiEngine::GunAction_LastAttack::UpdateGL_ShootWaitPhase()
{
}

void ButiEngine::GunAction_LastAttack::StartGL_ReturnCenterPhase()
{
}

void ButiEngine::GunAction_LastAttack::UpdateGL_ReturnCenterPhase()
{
}

void ButiEngine::GunAction_LastAttack::AddPositionAnimation(const Vector3& arg_targetPos, const float arg_speed, const Easing::EasingType arg_easeType)
{
	auto anim = gameObject.lock()->AddGameComponent<PositionAnimation>();
	anim->SetInitPosition(gameObject.lock()->transform->GetLocalPosition());
	anim->SetTargetPosition(arg_targetPos);
	anim->SetSpeed(arg_speed);
	anim->SetEaseType(arg_easeType);
}
