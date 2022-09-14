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
	if (m_vlp_invincibleTime->Update())
	{
		m_isInvincible = false;
	}

	if (m_isPause)
	{
		return;
	}

	Control();
}

void ButiEngine::LesserEnemy::OnShowUI()
{
	BaseEnemy::OnShowUI();
	GUI::Text("Derrived BaseEnemy");
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::LesserEnemy::Clone()
{
	auto output = ObjectFactory::Create<LesserEnemy>();
	output->m_speed = m_speed;
	output->m_defaultSpeed = m_defaultSpeed;
	output->m_directionDicisionInterval = m_directionDicisionInterval;
	output->m_defaultDirectionDicisionInterval = m_defaultDirectionDicisionInterval;
	output->m_minimumDistance = m_minimumDistance;
	output->m_hitPoint = m_hitPoint;
	output->m_maxHitPoint = m_maxHitPoint;
	output->m_invincibleInterval = m_invincibleInterval;
	output->m_attackInterval = m_attackInterval;
	return output;
}

void ButiEngine::LesserEnemy::Dead()
{
	gameObject.lock()->GetGameComponent<SeparateDrawObject>()->Dead();
	m_vwp_equipGunComponent.lock()->Dead();
	gameObject.lock()->SetIsRemove(true);
}

void ButiEngine::LesserEnemy::Control()
{
	//直前フレームにダメージがあったかを判定
	CheckHasDamageInPreviousFrame();

	Move();
	Rotate();
	Attack();
}

void ButiEngine::LesserEnemy::Move()
{
	//Playerに近づきたい
	if (m_vlp_directionDicisionTime->Update())
	{
		DecideDirection();
	}
	m_vwp_rigidBody.lock()->GetRigidBody()->SetVelocity(m_direction * m_speed * GameDevice::GetWorldSpeed());
}

void ButiEngine::LesserEnemy::Rotate()
{
	auto lookTarget = m_vwp_lookAt.lock()->GetLookTarget();
	Vector3 playerPos = m_vwp_player.lock()->transform->GetLocalPosition();
	lookTarget->SetLocalPosition(Vector3(playerPos.x, m_vwp_drawObject.lock()->transform->GetWorldPosition().y, playerPos.z));
}

void ButiEngine::LesserEnemy::DecideDirection()
{
	m_vlp_directionDicisionTime->Reset();

	//距離計算
	Value_weak_ptr<RigidBodyComponent> playerRigidBody = m_vwp_player.lock()->GetGameComponent<RigidBodyComponent>();
	Vector3 playerPosition = playerRigidBody.lock()->GetRigidBody()->GetPosition();
	Vector3 position = m_vwp_rigidBody.lock()->GetRigidBody()->GetPosition();
	float distance = playerPosition.Distance(position);

	if (distance <= m_minimumDistance)
	{
		m_speed = m_defaultSpeed * 0.5f;

		m_direction = Vector3(0, 0, 0);
		m_direction.Normalize();
	}
	else
	{
		m_speed = m_defaultSpeed;

		m_direction = Vector3(playerPosition.x - position.x, 0, playerPosition.z - position.z);
		m_direction.Normalize();
	}
}

void ButiEngine::LesserEnemy::Attack()
{
	//距離計算
	Value_weak_ptr<RigidBodyComponent> playerRigidBody = m_vwp_player.lock()->GetGameComponent<RigidBodyComponent>();
	Vector3 playerPosition = playerRigidBody.lock()->GetRigidBody()->GetPosition();
	Vector3 position = m_vwp_rigidBody.lock()->GetRigidBody()->GetPosition();
	float distance = playerPosition.Distance(position);

	if (distance <= m_minimumDistance * 1.5f)
	{
		m_vwp_gunComponent.lock()->ShootStart();
	}
	else
	{
		m_vwp_gunComponent.lock()->ShootStop();
	}
}

void ButiEngine::LesserEnemy::Damage(const int32_t arg_power)
{
	if (m_isInvincible)
	{
		return;
	}
	if (m_hitPoint <= 0)
	{
		return;
	}

	m_hitPoint -= arg_power;

	if (m_hitPoint <= 0)
	{
		EmitItem();
		Dead();
	}
}
