#include "stdafx_u.h"
#include "LesserEnemy.h"
#include "InputManager.h"
#include "Header/GameObjects/DefaultGameComponent/RigidBodyComponent.h"
#include "SeparateDrawObject.h"
#include "Gun.h"
#include "Player.h"

void ButiEngine::LesserEnemy::OnUpdate()
{
	if (m_vlp_directionDicisionInterval->Update())
	{
		DecideDirection();
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
	m_vlp_directionDicisionInterval = ObjectFactory::Create<RelativeTimer>(m_moveRate);
	m_vlp_directionDicisionInterval->Start();
	m_direction = Vector3();
	DecideDirection();
}

void ButiEngine::LesserEnemy::OnRemove()
{
}

void ButiEngine::LesserEnemy::OnShowUI()
{
	GUI::BulletText(u8"ˆÚ“®‘¬“x");
	GUI::DragFloat("##speed", &m_speed, 1.0f, 0.0f, 10.0f);

	GUI::BulletText(u8"ˆÚ“®•ûŒüŒˆ’èŠÔŠu");
	if (GUI::DragInt("##moveRate", &m_moveRate, 1.0f, 1.0f, 60.0f))
	{
		if (m_vlp_directionDicisionInterval)
		{
			m_vlp_directionDicisionInterval->ChangeCountFrame(m_moveRate);
		}
	}

	GUI::BulletText(u8"‹ßÚ‹——£–Ú•W");
	GUI::DragFloat("##minimumDistance", &m_minimumDistance, 1.0f, 0.0f, 100.0f);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::LesserEnemy::Clone()
{
	auto output = ObjectFactory::Create<LesserEnemy>();
	output->m_speed = m_speed;
	output->m_moveRate = m_moveRate;
	output->m_minimumDistance = m_minimumDistance;
	return output;
}

void ButiEngine::LesserEnemy::Dead()
{
	gameObject.lock()->GetGameComponent<SeparateDrawObject>()->Dead();
	gameObject.lock()->SetIsRemove(true);
}

void ButiEngine::LesserEnemy::Move()
{
	//Player‚É‹ß‚Ã‚«‚½‚¢
	m_vwp_rigidBody.lock()->GetRigidBody()->SetVelocity(m_direction * m_speed * GameDevice::WorldSpeed);
}

void ButiEngine::LesserEnemy::DecideDirection()
{
	m_vlp_directionDicisionInterval->Reset();

	//‹——£ŒvZ
	Value_weak_ptr<RigidBodyComponent> playerRigidBody = m_vwp_player.lock()->GetGameComponent<RigidBodyComponent>();
	Vector3 playerPosition = playerRigidBody.lock()->GetRigidBody()->GetPosition();
	Vector3 position = m_vwp_rigidBody.lock()->GetRigidBody()->GetPosition();
	float distance = playerPosition.Distance(position);
	
	if (distance <= m_minimumDistance)
	{
		float sin, cos;
		MathHelper::SinCos(sin, cos, MathHelper::ToRadian(ButiRandom::GetInt(0, 360)));
		m_direction = Vector3(cos, 0, sin);
		m_direction.Normalize();
	}
	else
	{
		m_direction = Vector3(playerPosition.x - position.x, 0, playerPosition.z - position.z);
		m_direction.Normalize();
	}
}

void ButiEngine::LesserEnemy::Shoot()
{
}

void ButiEngine::LesserEnemy::CreateGun()
{
}
