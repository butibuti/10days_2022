#include "stdafx_u.h"
#include "LesserEnemy.h"
#include "InputManager.h"
#include "Header/GameObjects/DefaultGameComponent/RigidBodyComponent.h"
#include "SeparateDrawObject.h"
#include "Gun.h"
#include "Player.h"

void ButiEngine::LesserEnemy::OnUpdate()
{
	if (m_vlp_velocityDicisionInterval->Update())
	{
		DecideVelocity();
	}
	Move();
}

void ButiEngine::LesserEnemy::OnSet()
{
}

void ButiEngine::LesserEnemy::Start()
{
	m_vwp_rigidBody = gameObject.lock()->GetGameComponent<RigidBodyComponent>();
	m_vwp_drawObjectTransform = gameObject.lock()->GetGameComponent<SeparateDrawObject>()->GetDrawObject().lock()->transform;
	m_vwp_player = GetManager().lock()->GetGameObject("Player");
	m_vlp_velocityDicisionInterval = ObjectFactory::Create<RelativeTimer>(m_moveRate);
	m_vlp_velocityDicisionInterval->Start();
	m_velocity = Vector3();
	DecideVelocity();
}

void ButiEngine::LesserEnemy::OnRemove()
{
}

void ButiEngine::LesserEnemy::OnShowUI()
{
	GUI::BulletText(u8"移動速度");
	GUI::DragFloat("##speed", &m_speed, 1.0f, 0.0f, 10.0f);

	GUI::BulletText(u8"移動方向決定間隔");
	if (GUI::DragInt("##moveRate", &m_moveRate, 1.0f, 1.0f, 60.0f))
	{
		if (m_vlp_velocityDicisionInterval)
		{
			m_vlp_velocityDicisionInterval->ChangeCountFrame(m_moveRate);
		}
	}
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::LesserEnemy::Clone()
{
	return ObjectFactory::Create<LesserEnemy>();
}

void ButiEngine::LesserEnemy::Dead()
{
	gameObject.lock()->GetGameComponent<SeparateDrawObject>()->Dead();
	gameObject.lock()->SetIsRemove(true);
}

void ButiEngine::LesserEnemy::Move()
{
	//Playerに近づきたい
	/*Vector2 rightStick = InputManager::GetRightStick();
	Vector3 velocity = Vector3(rightStick.x, 0.0f, rightStick.y).Normalize() * speed;*/
	m_vwp_rigidBody.lock()->GetRigidBody()->SetVelocity(m_velocity * m_speed * GameDevice::WorldSpeed);
}

void ButiEngine::LesserEnemy::DecideVelocity()
{
	m_vlp_velocityDicisionInterval->Reset();

	//距離計算
	Value_weak_ptr<RigidBodyComponent> playerRigidBody = m_vwp_player.lock()->GetGameComponent<RigidBodyComponent>();
	Vector3 playerPosition = playerRigidBody.lock()->GetRigidBody()->GetPosition();
	Vector3 position = m_vwp_rigidBody.lock()->GetRigidBody()->GetPosition();
	float distance = playerPosition.Distance(position);
	
	float minimumDistance = 3.0f;
	if (distance <= minimumDistance)
	{
		//ランダムにしたい
	}
	else
	{
		m_velocity = Vector3(playerPosition.x - position.x, 0, playerPosition.z - position.z);
	}
}

void ButiEngine::LesserEnemy::Shoot()
{
}

void ButiEngine::LesserEnemy::CreateGun()
{
}
