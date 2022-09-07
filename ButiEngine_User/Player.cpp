#include "stdafx_u.h"
#include "Player.h"
#include "InputManager.h"
#include "Header/GameObjects/DefaultGameComponent/RigidBodyComponent.h"
#include "SeparateDrawObject.h"
#include "Gun.h"

void ButiEngine::Player::OnUpdate()
{
	Move();
	Shoot();
}

void ButiEngine::Player::OnSet()
{
}

void ButiEngine::Player::Start()
{
	m_vwp_rigidBody = gameObject.lock()->GetGameComponent<RigidBodyComponent>();

	m_vwp_drawObjectTransform = gameObject.lock()->GetGameComponent<SeparateDrawObject>()->GetDrawObject().lock()->transform;
}

void ButiEngine::Player::OnRemove()
{
}

void ButiEngine::Player::OnShowUI()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Player::Clone()
{
	return ObjectFactory::Create<Player>();
}

void ButiEngine::Player::Dead()
{
	gameObject.lock()->GetGameComponent<SeparateDrawObject>()->Dead();
	m_vwp_gun.lock()->SetIsRemove(true);
	gameObject.lock()->SetIsRemove(true);
}

void ButiEngine::Player::Move()
{
	Vector2 leftStick = InputManager::GetLeftStick();
	float speed = 5.0f;
	Vector3 velocity = Vector3(leftStick.x, 0.0f, leftStick.y).Normalize() * speed;
	m_vwp_rigidBody.lock()->GetRigidBody()->SetVelocity(velocity * GameDevice::WorldSpeed);
}

void ButiEngine::Player::Shoot()
{
	//if (InputManager::IsPushShootKey())
	//{
	//	m_vwp_gunComponent.lock()->Shoot();
	//}
}
