#include "stdafx_u.h"
#include "GunAction_LastAttack.h"
#include "EquipGun.h"
#include "Gun.h"
#include "SeparateDrawObject.h"
#include "Player.h"
#include "Camera.h"
#include "Header/GameObjects/DefaultGameComponent/PositionAnimationComponent.h"
#include "Header/GameObjects/DefaultGameComponent/RotationAnimationComponent.h"

void ButiEngine::GunAction_LastAttack::OnUpdate()
{
	switch (m_phase)
	{
	case ButiEngine::GunAction_LastAttackPhase::MoveToBoss:
		UpdateMoveToBossPhase();
		break;
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
	case ButiEngine::GunAction_LastAttackPhase::GL_Wait:
		UpdateGL_WaitPhase();
		break;
	default:
		break;
	}
}

void ButiEngine::GunAction_LastAttack::OnSet()
{
	GetManager().lock()->GetGameObject("CameraMan").lock()->GetGameComponent<Camera>()->StartChasePlayer();

	m_vwp_drawObject = gameObject.lock()->GetGameComponent<SeparateDrawObject>()->GetDrawObject();
	auto lookAt = m_vwp_drawObject.lock()->GetGameComponent<LookAtComponent>();
	lookAt->SetIsActive(true);
	lookAt->SetSpeed(0.3f);
	

	auto equipGunComponent = gameObject.lock()->GetGameComponent<EquipGun>();
	m_vwp_rightGunComponent = equipGunComponent->GetGun(0).lock()->GetGameComponent<Gun>();
	m_vwp_leftGunComponent = equipGunComponent->GetGun(1).lock()->GetGameComponent<Gun>();

	m_vwp_boss = GetManager().lock()->GetGameObject("BossEnemy");
	Vector3 lookPos = m_vwp_boss.lock()->transform->GetLocalPosition();
	lookPos.y = gameObject.lock()->transform->GetLocalPosition().y;
	lookAt->GetLookTarget()->SetLocalPosition(lookPos);

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

	m_vlp_moveToBossPhaseTimer = ObjectFactory::Create<RelativeTimer>(20);

	m_vlp_AR_firstMovePhaseTimer = ObjectFactory::Create<RelativeTimer>(20);

	m_vlp_AR_firstShootPhaseTimer = ObjectFactory::Create<RelativeTimer>(30);

	m_vlp_AR_secondMovePhaseTimer = ObjectFactory::Create<RelativeTimer>(20);

	m_vlp_AR_secondShootPhaseTimer = ObjectFactory::Create<RelativeTimer>(30);

	m_vlp_AR_thirdMovePhaseTimer = ObjectFactory::Create<RelativeTimer>(20);

	m_vlp_AR_thirdShootPhaseTimer = ObjectFactory::Create<RelativeTimer>(60);

	m_vlp_AR_SG_waitPhaseTimer = ObjectFactory::Create<RelativeTimer>(60);

	m_vlp_SG_shootPhaseTimer = ObjectFactory::Create<RelativeTimer>(90);
	m_vlp_moveShootCenterTransform = ObjectFactory::Create<Transform>(m_vwp_boss.lock()->transform->GetLocalPosition());
	m_vlp_moveShootCenterTransform->SetLocalPositionY(gameObject.lock()->transform->GetLocalPosition().y);
	m_maxShootCount = 10;
	m_shootCount = 0;
	m_vlp_moveShootTimer = ObjectFactory::Create<RelativeTimer>(m_vlp_SG_shootPhaseTimer->GetMaxCountFrame() / m_maxShootCount);
	m_radius = 7.5f;

	m_vlp_SG_GL_waitPhaseTimer = ObjectFactory::Create<RelativeTimer>(60);

	m_vlp_GL_moveOffScreenPhaseTimer = ObjectFactory::Create<RelativeTimer>(20);

	m_vlp_GL_moveOffScreenWaitPhaseTimer = ObjectFactory::Create<RelativeTimer>(30);

	m_vlp_GL_firstShootPhaseTimer = ObjectFactory::Create<RelativeTimer>(40);

	m_vlp_GL_secondShootPhaseTimer = ObjectFactory::Create<RelativeTimer>(30);

	m_vlp_GL_shootWaitPhaseTimer = ObjectFactory::Create<RelativeTimer>(30);

	m_vlp_GL_returnCenterPhaseTimer = ObjectFactory::Create<RelativeTimer>(30);

	m_vlp_GL_waitPhaseTimer = ObjectFactory::Create<RelativeTimer>(60);

	StartMoveToBossPhase();
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

void ButiEngine::GunAction_LastAttack::StartMoveToBossPhase()
{
	m_phase = GunAction_LastAttackPhase::MoveToBoss;

	Vector3 bossPos = m_vwp_boss.lock()->transform->GetLocalPosition();
	Vector3 targetPos = bossPos;
	targetPos.y = gameObject.lock()->transform->GetLocalPosition().y;
	targetPos.z -= 4.0f;
	AddPositionAnimation(targetPos, 1.0f / m_vlp_moveToBossPhaseTimer->GetMaxCountFrame(), Easing::EasingType::EaseOutCirc);

	m_vlp_moveToBossPhaseTimer->Start();
}

void ButiEngine::GunAction_LastAttack::UpdateMoveToBossPhase()
{
	if (m_vlp_moveToBossPhaseTimer->Update())
	{
		m_vlp_moveToBossPhaseTimer->Stop();
		GetManager().lock()->GetGameObject("CameraMan").lock()->GetGameComponent<Camera>()->StopChasePlayer();
		StartAR_FirstMovePhase();
	}
}

void ButiEngine::GunAction_LastAttack::StartAR_FirstMovePhase()
{
	m_phase = GunAction_LastAttackPhase::AR_FirstMove;

	m_vlp_startTransform = gameObject.lock()->transform->Clone();
	m_vlp_startTransform->SetLocalRotation(m_vwp_drawObject.lock()->transform->GetLocalRotation());

	Vector3 targetPos = m_vlp_startTransform->GetLocalPosition();
	targetPos.x += 5.0f;
	targetPos.z += 1.5f;
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
	Vector3 targetPos = m_vlp_startTransform->GetLocalPosition();
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
	Vector3 targetPos = m_vlp_startTransform->GetLocalPosition();
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
	m_phase = GunAction_LastAttackPhase::AR_SG_Wait;
	m_vwp_rightGunComponent = gameObject.lock()->GetGameComponent<Player>()->ChangeGun("Gun_Player_SpecialShotgun_Right", true).lock()->GetGameComponent<Gun>();
	m_vwp_leftGunComponent = gameObject.lock()->GetGameComponent<EquipGun>()->ChangeGun("Gun_Player_SpecialShotgun_Left", 1, true).lock()->GetGameComponent<Gun>();
	m_vlp_AR_SG_waitPhaseTimer->Start();
}

void ButiEngine::GunAction_LastAttack::UpdateAR_SG_WaitPhase()
{
	if (m_vlp_AR_SG_waitPhaseTimer->Update())
	{
		m_vlp_AR_SG_waitPhaseTimer->Stop();
		StartSG_ShootPhase();
	}
}

void ButiEngine::GunAction_LastAttack::StartSG_ShootPhase()
{
	m_phase = GunAction_LastAttackPhase::SG_Shoot;
	m_vwp_rightGunComponent.lock()->ShootStart();

	m_vlp_moveShootCenterTransform->SetLookAtRotation(gameObject.lock()->transform->GetLocalPosition());
	SG_AddShootAnimation();

	m_vlp_moveShootTimer->Start();
	m_vlp_SG_shootPhaseTimer->Start();
}

void ButiEngine::GunAction_LastAttack::UpdateSG_ShootPhase()
{
	if (m_vlp_SG_shootPhaseTimer->Update())
	{
		m_vlp_SG_shootPhaseTimer->Stop();
		m_vlp_moveShootTimer->Stop();
		m_vwp_rightGunComponent.lock()->ShootStop();
		m_vwp_leftGunComponent.lock()->ShootStop();
		StartSG_GLWaitPhase();
	}

	if (m_vlp_moveShootTimer->Update())
	{
		if (m_shootCount == 1)
		{
			m_vwp_leftGunComponent.lock()->ShootStart();
		}
		SG_AddShootAnimation();
	}
}

void ButiEngine::GunAction_LastAttack::StartSG_GLWaitPhase()
{
	m_phase = GunAction_LastAttackPhase::SG_GL_Wait;
	m_vwp_rightGunComponent = gameObject.lock()->GetGameComponent<Player>()->ChangeGun("Gun_Player_SpecialGrenadeLauncher_Right", true).lock()->GetGameComponent<Gun>();
	m_vwp_leftGunComponent = gameObject.lock()->GetGameComponent<EquipGun>()->ChangeGun("Gun_Player_SpecialGrenadeLauncher_Left", 1, true).lock()->GetGameComponent<Gun>();
	m_vlp_SG_GL_waitPhaseTimer->Start();
}

void ButiEngine::GunAction_LastAttack::UpdateSG_GLWaitPhase()
{
	if (m_vlp_SG_GL_waitPhaseTimer->Update())
	{
		m_vlp_SG_GL_waitPhaseTimer->Stop();
		StartGL_MoveOffScreenPhase();
	}
}

void ButiEngine::GunAction_LastAttack::StartGL_MoveOffScreenPhase()
{
	m_phase = GunAction_LastAttackPhase::GL_MoveOffScreen;
	m_vwp_drawObject.lock()->GetGameComponent<LookAtComponent>()->SetIsActive(false);

	auto target = m_vlp_startTransform->Clone();
	target->TranslateX(-25.0f);
	target->SetLocalRotationY_Degrees(-90.0f);

	auto rotAnim = m_vwp_drawObject.lock()->AddGameComponent<RotationAnimation>();
	rotAnim->SetInitRotate(m_vlp_startTransform->GetLocalRotation());
	rotAnim->SetTargetRotate(target->GetLocalRotation());
	rotAnim->SetSpeed((1.0f / m_vlp_GL_moveOffScreenPhaseTimer->GetMaxCountFrame()) * 2.0f);
	rotAnim->SetEaseType(Easing::EasingType::EaseOutCirc);

	AddPositionAnimation(target->GetLocalPosition(), 1.0f / m_vlp_GL_moveOffScreenPhaseTimer->GetMaxCountFrame(), Easing::EasingType::EaseOutCirc);

	m_vlp_GL_moveOffScreenPhaseTimer->Start();
}

void ButiEngine::GunAction_LastAttack::UpdateGL_MoveOffScreenPhase()
{
	if (m_vlp_GL_moveOffScreenPhaseTimer->Update())
	{
		m_vlp_GL_moveOffScreenPhaseTimer->Stop();
		StartGL_MoveOffScreenWaitPhase();
	}
}

void ButiEngine::GunAction_LastAttack::StartGL_MoveOffScreenWaitPhase()
{
	m_phase = GunAction_LastAttackPhase::GL_MoveOffScreenWait;
	m_vlp_GL_moveOffScreenWaitPhaseTimer->Start();
}

void ButiEngine::GunAction_LastAttack::UpdateGL_MoveOffScreenWaitPhase()
{
	if (m_vlp_GL_moveOffScreenWaitPhaseTimer->Update())
	{
		m_vlp_GL_moveOffScreenWaitPhaseTimer->Stop();
		StartGL_FirstShootPhase();
	}
}

void ButiEngine::GunAction_LastAttack::StartGL_FirstShootPhase()
{
	m_phase = GunAction_LastAttackPhase::GL_FirstShoot;

	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();
	Vector3 bossPos = m_vwp_boss.lock()->transform->GetLocalPosition();

	gameObject.lock()->transform->SetLocalPosition(Vector3(pos.x, pos.y, bossPos.z));

	m_vwp_drawObject.lock()->transform->SetLocalRotationY_Degrees(90.0f);

	Vector3 targetPos = gameObject.lock()->transform->GetLocalPosition();
	targetPos.x += 50.0f;
	AddPositionAnimation(targetPos, 1.0f / m_vlp_GL_firstShootPhaseTimer->GetMaxCountFrame(), Easing::EasingType::Liner);

	m_vwp_rightGunComponent.lock()->ShootStart();

	m_vlp_GL_firstShootPhaseTimer->Start();
}

void ButiEngine::GunAction_LastAttack::UpdateGL_FirstShootPhase()
{
	if (m_vlp_GL_firstShootPhaseTimer->GetCurrentCountFrame() == m_vwp_leftGunComponent.lock()->GetShootIntervalFrame() * 0.5f)
	{
		m_vwp_leftGunComponent.lock()->ShootStart();
	}
	if (m_vlp_GL_firstShootPhaseTimer->Update())
	{
		m_vlp_GL_firstShootPhaseTimer->Stop();
		StartGL_SecondShootPhase();
	}
}

void ButiEngine::GunAction_LastAttack::StartGL_SecondShootPhase()
{
	m_phase = GunAction_LastAttackPhase::GL_SecondShoot;

	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();
	Vector3 bossPos = m_vwp_boss.lock()->transform->GetLocalPosition();

	gameObject.lock()->transform->SetLocalPosition(Vector3(bossPos.x, pos.y, bossPos.z + 15.0f));

	m_vwp_drawObject.lock()->transform->SetLocalRotationY_Degrees(180.0f);

	Vector3 targetPos = gameObject.lock()->transform->GetLocalPosition();
	targetPos.z -= 35.0f;
	AddPositionAnimation(targetPos, 1.0f / m_vlp_GL_secondShootPhaseTimer->GetMaxCountFrame(), Easing::EasingType::Liner);

	m_vlp_GL_secondShootPhaseTimer->Start();
}

void ButiEngine::GunAction_LastAttack::UpdateGL_SecondShootPhase()
{
	if (m_vlp_GL_secondShootPhaseTimer->GetCurrentCountFrame() == m_vwp_leftGunComponent.lock()->GetShootIntervalFrame() * 0.5f)
	{
		m_vwp_leftGunComponent.lock()->ShootStart();
	}
	if (m_vlp_GL_secondShootPhaseTimer->Update())
	{
		m_vlp_GL_secondShootPhaseTimer->Stop();
		m_vwp_rightGunComponent.lock()->ShootStop();
		m_vwp_leftGunComponent.lock()->ShootStop();
		StartGL_ShootWaitPhase();
	}
}

void ButiEngine::GunAction_LastAttack::StartGL_ShootWaitPhase()
{
	m_phase = GunAction_LastAttackPhase::GL_ShootWait;
	m_vlp_GL_shootWaitPhaseTimer->Start();
}

void ButiEngine::GunAction_LastAttack::UpdateGL_ShootWaitPhase()
{
	if (m_vlp_GL_shootWaitPhaseTimer->Update())
	{
		m_vlp_GL_shootWaitPhaseTimer->Stop();
		StartGL_ReturnCenterPhase();
	}
}

void ButiEngine::GunAction_LastAttack::StartGL_ReturnCenterPhase()
{
	m_phase = GunAction_LastAttackPhase::GL_ReturnCenter;

	m_vwp_drawObject.lock()->transform->SetLocalRotationY_Degrees(0.0f);

	AddPositionAnimation(m_vlp_startTransform->GetLocalPosition(), 1.0f / m_vlp_GL_returnCenterPhaseTimer->GetMaxCountFrame(), Easing::EasingType::EaseOutExpo);

	m_vlp_GL_returnCenterPhaseTimer->Start();
}

void ButiEngine::GunAction_LastAttack::UpdateGL_ReturnCenterPhase()
{
	if (m_vlp_GL_returnCenterPhaseTimer->Update())
	{
		m_vlp_GL_returnCenterPhaseTimer->Stop();
		gameObject.lock()->GetGameComponent<Player>()->ChangeGun("Gun_Player_Normal", true);
		auto equipGunComponent = gameObject.lock()->GetGameComponent<EquipGun>();
		gameObject.lock()->GetGameComponent<EquipGun>()->RemoveGun(1, true);
		StartGL_WaitPhase();
		return;
	}
}

void ButiEngine::GunAction_LastAttack::StartGL_WaitPhase()
{
	m_phase = GunAction_LastAttackPhase::GL_Wait;
	m_vlp_GL_waitPhaseTimer->Start();
}

void ButiEngine::GunAction_LastAttack::UpdateGL_WaitPhase()
{
	if (m_vlp_GL_waitPhaseTimer->Update())
	{
		m_vlp_GL_waitPhaseTimer->Stop();
		SetIsRemove(true);
	}
}

void ButiEngine::GunAction_LastAttack::AddPositionAnimation(const Vector3& arg_targetPos, const float arg_speed, const Easing::EasingType arg_easeType)
{
	auto anim = gameObject.lock()->AddGameComponent<PositionAnimation>();
	anim->SetInitPosition(gameObject.lock()->transform->GetLocalPosition());
	anim->SetTargetPosition(arg_targetPos);
	anim->SetSpeed(arg_speed);
	anim->SetEaseType(arg_easeType);
}

void ButiEngine::GunAction_LastAttack::SG_AddShootAnimation()
{
	m_shootCount++;

	m_vlp_moveShootCenterTransform->RollLocalRotationY_Degrees(360.0f / m_maxShootCount);

	auto anim = gameObject.lock()->AddGameComponent<PositionAnimation>();
	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();
	anim->SetInitPosition(pos);
	Vector3 targetPos = m_vlp_moveShootCenterTransform->GetLocalPosition() + m_vlp_moveShootCenterTransform->GetFront() * m_radius;
	anim->SetTargetPosition(targetPos);
	anim->SetSpeed(1.0f / m_vlp_moveShootTimer->GetMaxCountFrame());
	anim->SetEaseType(Easing::EasingType::EaseOutCirc);
}
