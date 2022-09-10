#include "stdafx_u.h"
#include "GunAction_GrenadeLauncher.h"

void ButiEngine::GunAction_GrenadeLauncher::OnUpdate()
{
}

void ButiEngine::GunAction_GrenadeLauncher::OnSet()
{
}

void ButiEngine::GunAction_GrenadeLauncher::Start()
{
	//std::int32_t rotateFrame = 60;
	//m_vlp_rotateTimer = ObjectFactory::Create<RelativeTimer>(rotateFrame);

	//std::int32_t shootFrame = 
	//m_vlp_shootTimer = ObjectFactory::Create<RelativeTimer>(shootFrames[m_phase]);

	//m_vwp_drawObject = gameObject.lock()->GetGameComponent<SeparateDrawObject>()->GetDrawObject();
	//m_vwp_lookTarget = m_vwp_drawObject.lock()->GetGameComponent<LookAtComponent>()->GetLookTarget();

	//m_vwp_playerComponent = gameObject.lock()->GetGameComponent<Player>();
	//m_vwp_playerComponent.lock()->SetCanAcceptInput(false);
	//m_vwp_playerComponent.lock()->SetIsInvincible(true);

	//gameObject.lock()->GetGameComponent<RigidBodyComponent>()->GetRigidBody()->SetVelocity(Vector3Const::Zero);

	//m_vwp_gunComponent = m_vwp_playerComponent.lock()->ChangeGun("Gun_Player_AssaultRifle");

	//RotateStart();
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
