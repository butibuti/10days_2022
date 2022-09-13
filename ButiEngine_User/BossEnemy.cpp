#include "stdafx_u.h"
#include "BossEnemy.h"
#include "InputManager.h"
#include "Header/GameObjects/DefaultGameComponent/RigidBodyComponent.h"
#include "SeparateDrawObject.h"
#include "Gun.h"
#include "EquipGun.h"
#include "Player.h"
#include "GunAction_BossSpin.h"
#include "GunAction_BossSnake.h"
#include "ItemEmitParameter.h"

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
	if (!m_canMove)
	{
		return;
	}
	if (m_isPassedOut)
	{
		int a = 5;
		a++;
		return;
	}

	Control();
}

void ButiEngine::BossEnemy::Start()
{
	BaseEnemy::Start();

	m_canMove = true;
	m_isPassedOut = false;
}

void ButiEngine::BossEnemy::OnShowUI()
{
	BaseEnemy::OnShowUI();

	GUI::Text("------------");
	GUI::BulletText(u8"attackTime");
	GUI::Text(m_vlp_attackTime->GetCurrentCountFrame());
	GUI::BulletText(u8"directionDicisionTime");
	GUI::Text(m_vlp_directionDicisionTime->GetCurrentCountFrame());
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
	/*gameObject.lock()->GetGameComponent<SeparateDrawObject>()->Dead();
	m_vwp_equipGunComponent.lock()->Dead();
	gameObject.lock()->SetIsRemove(true);*/

	EmitItem();
	StartPause();
	m_isPause = false;
	m_canMove = true;
	m_isPassedOut = true;

	//特殊攻撃中ならコンポーネント削除
	auto bossSpinComponent = gameObject.lock()->GetGameComponent<GunAction_BossSpin>();
	if (bossSpinComponent)
	{
		bossSpinComponent->SetIsRemove(true);
	}
	auto bossSnakeComponent = gameObject.lock()->GetGameComponent<GunAction_BossSnake>();
	if (bossSnakeComponent)
	{
		bossSnakeComponent->SetIsRemove(true);
	}
}

void ButiEngine::BossEnemy::StartGunAction()
{
	m_canMove = false;
	m_vwp_gunComponent.lock()->ShootStop();
	m_vlp_attackTime->Stop();
	m_vlp_directionDicisionTime->Stop();
}

void ButiEngine::BossEnemy::FinishGunAction()
{
	m_canMove = true;
	m_vlp_attackTime->Start();
	m_vlp_directionDicisionTime->Start();
	m_vwp_lookAt.lock()->SetIsActive(true);
}

void ButiEngine::BossEnemy::StartPause()
{
	m_isPause = true;
	m_vlp_attackTime->Stop();
	m_vlp_directionDicisionTime->Stop();
	m_vwp_gunComponent.lock()->ShootStop();
	m_vwp_rigidBody.lock()->GetRigidBody()->SetVelocity(Vector3());
	m_vwp_lookAt.lock()->SetIsActive(false);

	//特殊攻撃の中断
	auto bossSpinComponent = gameObject.lock()->GetGameComponent<GunAction_BossSpin>();
	if (bossSpinComponent)
	{
		bossSpinComponent->StartPause();
	}
	auto bossSnakeComponent = gameObject.lock()->GetGameComponent<GunAction_BossSnake>();
	if (bossSnakeComponent)
	{
		bossSnakeComponent->StartPause();
	}
}

void ButiEngine::BossEnemy::FinishPause()
{
	if (!m_isPause)
	{
		return;
	}

	m_isPause = false;
	m_vlp_attackTime->Start();
	m_vlp_directionDicisionTime->Start();
	m_vwp_lookAt.lock()->SetIsActive(true);

	//特殊攻撃の再開
	auto bossSpinComponent = gameObject.lock()->GetGameComponent<GunAction_BossSpin>();
	if (bossSpinComponent)
	{
		bossSpinComponent->FinishPause();
		m_vwp_lookAt.lock()->SetIsActive(false);
		m_vlp_attackTime->Stop();
		m_vlp_directionDicisionTime->Stop();
	}
	auto bossSnakeComponent = gameObject.lock()->GetGameComponent<GunAction_BossSnake>();
	if (bossSnakeComponent)
	{
		bossSnakeComponent->FinishPause();
		m_vlp_attackTime->Stop();
		m_vlp_directionDicisionTime->Stop();
	}
}

void ButiEngine::BossEnemy::Control()
{
	//直前フレームにダメージがあったかを判定
	CheckHasDamageInPreviousFrame();

	Move();
	//Rotate();
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

		if (m_vlp_attackTime->Update())
		{
			int32_t rnd = ButiRandom::GetInt(0, 1);
			if (rnd == 0)
			{
				gameObject.lock()->AddGameComponent<GunAction_BossSpin>();
			}
			else if (rnd == 1)
			{
				gameObject.lock()->AddGameComponent<GunAction_BossSnake>();
			}
		}
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

void ButiEngine::BossEnemy::EmitItem()
{
	Value_weak_ptr<ItemEmitParameter> vwp_itemEmitParameterComponent = gameObject.lock()->GetGameComponent<ItemEmitParameter>();
	if (!vwp_itemEmitParameterComponent.lock())
	{
		return;
	}
	std::string emitType = vwp_itemEmitParameterComponent.lock()->CalculateItemEmitType();

	if (emitType == "LastAttackItem")
	{
		auto item = GetManager().lock()->AddObjectFromCereal("LastAttackItem");
		item.lock()->transform->SetLocalPosition(gameObject.lock()->transform->GetWorldPosition() + Vector3(0, 0, -3));
	}
}
