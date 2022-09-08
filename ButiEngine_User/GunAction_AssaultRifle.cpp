#include "stdafx_u.h"
#include "GunAction_AssaultRifle.h"
#include "EquipGun.h"
#include "Gun.h"
#include "SeparateDrawObject.h"
#include "Player.h"
#include "Header/GameObjects/DefaultGameComponent/RigidBodyComponent.h"

std::int32_t constexpr shootFrames[3] = {30, 30, 60};
float constexpr rotationAngles[3] = { -30.0f, 60.0f, -30.0f };

void ButiEngine::GunAction_AssaultRifle::OnUpdate()
{
	if (m_vlp_rotateTimer->Update())
	{
		RotateStop();
		ShootStart();
	}

	if (m_vlp_shootTimer->Update())
	{
		ShootStop();

		m_phase++;
		std::int32_t constexpr maxPhaseNum = 2;
		if (m_phase > maxPhaseNum)
		{
			m_vwp_playerComponent.lock()->SetCanAcceptInput(true);
			m_vwp_playerComponent.lock()->SetIsInvincible(false);
			m_vwp_playerComponent.lock()->ChangeGun("Gun_Player_Normal");
			SetIsRemove(true);
			return;
		}

		RotateStart();
	}
}

void ButiEngine::GunAction_AssaultRifle::OnSet()
{
	m_phase = 0;

	std::int32_t waitFrame = 15;
	m_vlp_rotateTimer = ObjectFactory::Create<RelativeTimer>(waitFrame);
	m_vlp_shootTimer = ObjectFactory::Create<RelativeTimer>(shootFrames[m_phase]);

	m_vwp_drawObject = gameObject.lock()->GetGameComponent<SeparateDrawObject>()->GetDrawObject();
	m_vwp_lookTarget = m_vwp_drawObject.lock()->GetGameComponent<LookAtComponent>()->GetLookTarget();

	m_vwp_playerComponent = gameObject.lock()->GetGameComponent<Player>();
	m_vwp_playerComponent.lock()->SetCanAcceptInput(false);
	m_vwp_playerComponent.lock()->SetIsInvincible(true);

	gameObject.lock()->GetGameComponent<RigidBodyComponent>()->GetRigidBody()->SetVelocity(Vector3Const::Zero);

	m_vwp_gunComponent = m_vwp_playerComponent.lock()->ChangeGun("Gun_Player_AssaultRifle");

	RotateStart();
}

void ButiEngine::GunAction_AssaultRifle::Start()
{
}

void ButiEngine::GunAction_AssaultRifle::OnRemove()
{
}

void ButiEngine::GunAction_AssaultRifle::OnShowUI()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::GunAction_AssaultRifle::Clone()
{
	return ObjectFactory::Create<GunAction_AssaultRifle>();
}

void ButiEngine::GunAction_AssaultRifle::RotateStart()
{
	auto drawObjectTransform = m_vwp_drawObject.lock()->transform;
	Vector3 lookTargetPos = drawObjectTransform->GetWorldPosition() + drawObjectTransform->GetFront() * Matrix4x4::RollY(MathHelper::ToRadian(rotationAngles[m_phase]));
	m_vwp_lookTarget.lock()->SetLocalPosition(lookTargetPos);

	m_vlp_rotateTimer->Start();
}

void ButiEngine::GunAction_AssaultRifle::RotateStop()
{
	m_vlp_rotateTimer->Stop();
}

void ButiEngine::GunAction_AssaultRifle::ShootStart()
{
	m_vlp_shootTimer->ChangeCountFrame(shootFrames[m_phase]);
	m_vwp_gunComponent.lock()->ShootStart();
	m_vlp_shootTimer->Start();
}

void ButiEngine::GunAction_AssaultRifle::ShootStop()
{
	m_vwp_gunComponent.lock()->ShootStop();
	m_vlp_shootTimer->Stop();
}
