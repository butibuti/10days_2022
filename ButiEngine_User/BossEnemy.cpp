#include "stdafx_u.h"
#include "BossEnemy.h"
#include "InputManager.h"
#include "Header/GameObjects/DefaultGameComponent/RigidBodyComponent.h"
#include "SeparateDrawObject.h"
#include "Gun.h"
#include "EquipGun.h"
#include "Player.h"

void ButiEngine::BossEnemy::OnUpdate()
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

void ButiEngine::BossEnemy::OnShowUI()
{
	BaseEnemy::OnShowUI();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::BossEnemy::Clone()
{
	auto output = ObjectFactory::Create<BossEnemy>();
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

void ButiEngine::BossEnemy::Dead()
{
	gameObject.lock()->GetGameComponent<SeparateDrawObject>()->Dead();
	m_vwp_equipGunComponent.lock()->Dead();
	gameObject.lock()->SetIsRemove(true);
}

void ButiEngine::BossEnemy::Control()
{
	//直前フレームにダメージがあったかを判定
	CheckHasDamageInPreviousFrame();

	Move();
	Rotate();
	Attack();
}

void ButiEngine::BossEnemy::Move()
{
	//Playerに近づきたい
	if (m_vlp_directionDicisionTime->Update())
	{
		DecideDirection();
	}
	m_vwp_rigidBody.lock()->GetRigidBody()->SetVelocity(m_direction * m_speed * GameDevice::GetWorldSpeed());
}

void ButiEngine::BossEnemy::Rotate()
{
	auto lookTarget = m_vwp_lookAt.lock()->GetLookTarget();
	auto drawObjectTransform = m_vwp_drawObject.lock()->transform;
	lookTarget->SetLocalPosition(drawObjectTransform->GetLocalPosition() + drawObjectTransform->GetFront() * 100.0f);

	Vector3 playerPosition = m_vwp_player.lock()->GetGameComponent<RigidBodyComponent>()->GetRigidBody()->GetPosition();
	Vector3 position = m_vwp_rigidBody.lock()->GetRigidBody()->GetPosition();
	lookTarget->Translate(Vector3(playerPosition.x - position.x, 0.0f, playerPosition.z - position.z).Normalize() * 100.0f);
}

void ButiEngine::BossEnemy::DecideDirection()
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

		float sin, cos;
		MathHelper::SinCos(sin, cos, MathHelper::ToRadian(ButiRandom::GetInt(0, 360)));
		m_direction = Vector3(cos, 0, sin);
		m_direction.Normalize();
	}
	else
	{
		m_speed = m_defaultSpeed;

		m_direction = Vector3(playerPosition.x - position.x, 0, playerPosition.z - position.z);
		m_direction.Normalize();
	}
}

void ButiEngine::BossEnemy::Attack()
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

void ButiEngine::BossEnemy::Damage(const int32_t arg_power)
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
		Dead();
	}
}
