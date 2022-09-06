#include "stdafx_u.h"
#include "Player.h"
#include "InputManager.h"
#include "Header/GameObjects/DefaultGameComponent/RigidBodyComponent.h"

void ButiEngine::Player::OnUpdate()
{
	Vector2 leftStick = InputManager::GetLeftStick();
	float speed = 5.0f;
	Vector3 velocity = Vector3(leftStick.x, 0.0f, leftStick.y).Normalize() * speed;
	m_vwp_rigidBody.lock()->GetRigidBody()->SetVelocity(velocity * GameDevice::WorldSpeed);
}

void ButiEngine::Player::OnSet()
{
}

void ButiEngine::Player::Start()
{
	m_vwp_rigidBody = gameObject.lock()->GetGameComponent<RigidBodyComponent>();
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
