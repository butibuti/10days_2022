#include "stdafx_u.h"
#include "LesserEnemy.h"
#include "InputManager.h"
#include "Header/GameObjects/DefaultGameComponent/RigidBodyComponent.h"
#include "SeparateDrawObject.h"
#include "Gun.h"
#include "EquipGun.h"
#include "Player.h"

void ButiEngine::LesserEnemy::OnUpdate()
{
	Control();
}

void ButiEngine::LesserEnemy::OnSet()
{
}

void ButiEngine::LesserEnemy::Start()
{
	m_vwp_rigidBody = gameObject.lock()->GetGameComponent<RigidBodyComponent>();
	m_vwp_drawObject = gameObject.lock()->GetGameComponent<SeparateDrawObject>()->GetDrawObject().lock();
	m_vwp_equipGunComponent = gameObject.lock()->GetGameComponent<EquipGun>();
	m_vwp_gunComponent = m_vwp_equipGunComponent.lock()->GetGun().lock()->GetGameComponent<Gun>();
	m_vwp_player = GetManager().lock()->GetGameObject("Player");
	m_vlp_directionDicisionInterval = ObjectFactory::Create<RelativeTimer>(m_moveRate);
	m_vlp_directionDicisionInterval->Start();
	m_direction = Vector3();
	SetLookAtParameter();
	DecideDirection();
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
		if (m_vlp_directionDicisionInterval)
		{
			m_vlp_directionDicisionInterval->ChangeCountFrame(m_moveRate);
		}
	}

	GUI::BulletText(u8"近接距離目標");
	GUI::DragFloat("##minimumDistance", &m_minimumDistance, 1.0f, 0.0f, 100.0f);

	//デバッグ用表示
	GUI::Text(m_vwp_rigidBody.lock()->GetRigidBody()->GetPosition());
	GUI::Text(m_direction);
	GUI::Text(m_speed);
	GUI::Text(m_direction * m_speed);
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

void ButiEngine::LesserEnemy::Control()
{
	Move();
	Rotate();
	Shoot();
}

void ButiEngine::LesserEnemy::Move()
{
	//Playerに近づきたい
	if (m_vlp_directionDicisionInterval->Update())
	{
		DecideDirection();
	}
	m_vwp_rigidBody.lock()->GetRigidBody()->SetVelocity(m_direction * m_speed * GameDevice::WorldSpeed);
}

void ButiEngine::LesserEnemy::Rotate()
{
	auto lookTarget = m_vwp_lookAt.lock()->GetLookTarget();
	auto drawObjectTransform = m_vwp_drawObject.lock()->transform;
	lookTarget->SetLocalPosition(drawObjectTransform->GetLocalPosition() + drawObjectTransform->GetFront() * 100.0f);

	Vector3 playerPosition = m_vwp_player.lock()->GetGameComponent<RigidBodyComponent>()->GetRigidBody()->GetPosition();
	Vector3 position = m_vwp_rigidBody.lock()->GetRigidBody()->GetPosition();
	lookTarget->Translate(Vector3(playerPosition.x - position.x, 0.0f, playerPosition.z - position.z).Normalize() * 100.0f);
}

void ButiEngine::LesserEnemy::DecideDirection()
{
	m_vlp_directionDicisionInterval->Reset();

	//距離計算
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
	//距離計算
	Value_weak_ptr<RigidBodyComponent> playerRigidBody = m_vwp_player.lock()->GetGameComponent<RigidBodyComponent>();
	Vector3 playerPosition = playerRigidBody.lock()->GetRigidBody()->GetPosition();
	Vector3 position = m_vwp_rigidBody.lock()->GetRigidBody()->GetPosition();
	float distance = playerPosition.Distance(position);

	if (distance <= m_minimumDistance)
	{
		m_vwp_gunComponent.lock()->ShootStart();
	}
	else
	{
		m_vwp_gunComponent.lock()->ShootStop();
	}
}

void ButiEngine::LesserEnemy::SetLookAtParameter()
{
	m_vwp_lookAt = m_vwp_drawObject.lock()->GetGameComponent<LookAtComponent>();
	auto drawObjectTransform = m_vwp_drawObject.lock()->transform;
	m_vwp_lookAt.lock()->SetLookTarget(drawObjectTransform->Clone());
	m_vwp_lookAt.lock()->GetLookTarget()->Translate(drawObjectTransform->GetFront() * 100.0f);
	m_vwp_lookAt.lock()->SetSpeed(0.3f);
}
