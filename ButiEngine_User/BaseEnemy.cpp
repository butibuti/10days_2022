#include "stdafx_u.h"
#include "BaseEnemy.h"
#include "Header/GameObjects/DefaultGameComponent/RigidBodyComponent.h"
#include "SeparateDrawObject.h"
#include "Gun.h"
#include "EquipGun.h"
#include "Player.h"
#include "Bullet.h"
#include "ItemEmitParameter.h"
#include "DamageArea.h"

void ButiEngine::BaseEnemy::OnUpdate()
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

void ButiEngine::BaseEnemy::OnSet()
{
	//Bullet_Playerタグを持っていたらダメージを受ける
	gameObject.lock()->AddCollisionEnterReaction(
		[this](ButiBullet::ContactData& arg_other)
		{
			if (arg_other.vwp_gameObject.lock())
			{
				//タグ判定
				if (arg_other.vwp_gameObject.lock()->HasGameObjectTag("Bullet_Player") && !m_isInvincible)
				{
					auto bulletComponent = arg_other.vwp_gameObject.lock()->GetGameComponent<Bullet>();
					if (bulletComponent && !ExistInHitComponent(bulletComponent))
					{
						m_vec_vwp_hitComponent.push_back(bulletComponent);
						Damage(bulletComponent->GetPower());
					}
				}
				else if (arg_other.vwp_gameObject.lock()->HasGameObjectTag("DamageArea_Player") && !m_isInvincible)
				{
					auto damageArea = arg_other.vwp_gameObject.lock()->GetGameComponent<DamageArea>();
					if (damageArea && !ExistInHitComponent(damageArea))
					{
						m_vec_vwp_hitComponent.push_back(damageArea);
						Damage(damageArea->GetPower());
					}
				}
			}
		}
	);
}

void ButiEngine::BaseEnemy::Start()
{
	m_vwp_rigidBody = gameObject.lock()->GetGameComponent<RigidBodyComponent>();
	m_vwp_drawObject = gameObject.lock()->GetGameComponent<SeparateDrawObject>()->GetDrawObject().lock();
	m_vwp_equipGunComponent = gameObject.lock()->GetGameComponent<EquipGun>();
	m_vwp_gunComponent = m_vwp_equipGunComponent.lock()->GetGun().lock()->GetGameComponent<Gun>();
	m_vwp_player = GetManager().lock()->GetGameObject("Player");
	m_speed = m_defaultSpeed;
	m_directionDicisionInterval = m_defaultDirectionDicisionInterval;
	m_vlp_directionDicisionTime = ObjectFactory::Create<RelativeTimer>(m_directionDicisionInterval);
	m_vlp_directionDicisionTime->Start();
	m_hitPoint = m_maxHitPoint;
	m_vlp_invincibleTime = ObjectFactory::Create<RelativeTimer>(m_invincibleInterval);
	m_vlp_invincibleTime->Start();
	m_isInvincible = false;
	m_vec_vwp_hitComponent.clear();
	m_vlp_attackTime = ObjectFactory::Create<RelativeTimer>(m_attackInterval);
	m_vlp_attackTime->Start();
	m_vlp_attackTime->SetCount(1);
	m_direction = Vector3();
	m_isPause;
	SetLookAtParameter();
	DecideDirection();
}

void ButiEngine::BaseEnemy::OnRemove()
{
}

void ButiEngine::BaseEnemy::OnShowUI()
{
	GUI::BulletText(u8"移動速度");
	if (GUI::DragFloat("##defaultSpeed", &m_defaultSpeed, 1.0f, 0.0f, 20.0f))
	{
		m_speed = m_defaultSpeed;
	}
	
	GUI::BulletText(u8"移動方向決定間隔");
	if (GUI::DragInt("##defaultDirectionDicisionInterval", &m_defaultDirectionDicisionInterval, 1.0f, 1.0f, 60.0f))
	{
		m_directionDicisionInterval = m_defaultDirectionDicisionInterval;
		if (m_vlp_directionDicisionTime)
		{
			m_vlp_directionDicisionTime->ChangeCountFrame(m_directionDicisionInterval);
		}
	}
	
	GUI::BulletText(u8"近接距離目標");
	GUI::DragFloat("##minimumDistance", &m_minimumDistance, 1.0f, 0.0f, 100.0f);
	
	GUI::BulletText(u8"最大体力");
	if (GUI::DragInt("##maxHitPoint", &m_maxHitPoint))
	{
		m_hitPoint = m_maxHitPoint;
	}
	GUI::Text(m_hitPoint);
	
	GUI::BulletText(u8"無敵時間");
	if (GUI::DragInt("##invincibleInterval", &m_invincibleInterval, 1.0f, 1.0f, 30.0f))
	{
		if (m_vlp_invincibleTime)
		{
			m_vlp_invincibleTime->ChangeCountFrame(m_invincibleInterval);
		}
	}
	
	GUI::BulletText(u8"攻撃間隔");
	if (GUI::DragInt("##attackInterval", &m_attackInterval, 1.0f, 1.0f, 120.0f))
	{
		if (m_vlp_attackTime)
		{
			m_vlp_attackTime->ChangeCountFrame(m_attackInterval);
		}
	}
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::BaseEnemy::Clone()
{
	auto output = ObjectFactory::Create<BaseEnemy>();
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

void ButiEngine::BaseEnemy::Dead()
{
	gameObject.lock()->GetGameComponent<SeparateDrawObject>()->Dead();
	m_vwp_equipGunComponent.lock()->Dead();
	gameObject.lock()->SetIsRemove(true);
}

ButiEngine::Value_weak_ptr<ButiEngine::Gun> ButiEngine::BaseEnemy::ChangeGun(const std::string& arg_gunName)
{
	auto newGun = m_vwp_equipGunComponent.lock()->ChangeGun(arg_gunName, 0, true);
	m_vwp_gunComponent = newGun.lock()->GetGameComponent<Gun>();
	return m_vwp_gunComponent;
}

void ButiEngine::BaseEnemy::StartPause()
{
	m_isPause = true;
	m_vlp_attackTime->Stop();
	m_vlp_directionDicisionTime->Stop();
	m_vwp_gunComponent.lock()->ShootStop();
	m_vwp_rigidBody.lock()->GetRigidBody()->SetVelocity(Vector3());
	m_vwp_lookAt.lock()->SetIsActive(false);
}

void ButiEngine::BaseEnemy::FinishPause()
{
	if (!m_isPause)
	{
		return;
	}

	m_isPause = false;
	m_vlp_attackTime->Start();
	m_vlp_attackTime->SetCount(1);
	m_vlp_directionDicisionTime->Start();
	m_vwp_lookAt.lock()->SetIsActive(true);
}

void ButiEngine::BaseEnemy::Control()
{
	//直前フレームにダメージがあったかを判定
	CheckHasDamageInPreviousFrame();

	Move();
	Rotate();
	Attack();
}

void ButiEngine::BaseEnemy::Move()
{
	//Playerに近づきたい
	if (m_vlp_directionDicisionTime->Update())
	{
		DecideDirection();
	}
	m_vwp_rigidBody.lock()->GetRigidBody()->SetVelocity(m_direction * m_speed * GameDevice::GetWorldSpeed());
}

void ButiEngine::BaseEnemy::Rotate()
{
	auto lookTarget = m_vwp_lookAt.lock()->GetLookTarget();
	Vector3 playerPos = m_vwp_player.lock()->transform->GetLocalPosition();
	lookTarget->SetLocalPosition(Vector3(playerPos.x, m_vwp_drawObject.lock()->transform->GetWorldPosition().y, playerPos.z));
}

void ButiEngine::BaseEnemy::DecideDirection()
{
	m_vlp_directionDicisionTime->Reset();

	//距離計算
	Value_weak_ptr<RigidBodyComponent> playerRigidBody = m_vwp_player.lock()->GetGameComponent<RigidBodyComponent>();
	Vector3 playerPosition = playerRigidBody.lock()->GetRigidBody()->GetPosition();
	Vector3 position = m_vwp_rigidBody.lock()->GetRigidBody()->GetPosition();
	float distance = playerPosition.Distance(position);

	/*if (distance <= m_minimumDistance)
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
	}*/
	m_direction = Vector3(playerPosition.x - position.x, 0, playerPosition.z - position.z);
	m_direction.Normalize();
}

void ButiEngine::BaseEnemy::Attack()
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

void ButiEngine::BaseEnemy::Damage(const int32_t arg_power)
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

void ButiEngine::BaseEnemy::EmitItem()
{
	Value_weak_ptr<ItemEmitParameter> vwp_itemEmitParameterComponent = gameObject.lock()->GetGameComponent<ItemEmitParameter>();
	if (!vwp_itemEmitParameterComponent.lock())
	{
		return;
	}
	std::string emitType = vwp_itemEmitParameterComponent.lock()->CalculateItemEmitType();
	
	if (emitType == "BaseItem")
	{
		auto item = GetManager().lock()->AddObjectFromCereal("BaseItem");
		item.lock()->transform->SetLocalPosition(gameObject.lock()->transform->GetWorldPosition());
		AddObjectFromCereal("SphereEffect", ObjectFactory::Create<Transform>(gameObject.lock()->transform->GetWorldPosition()));
	}
	else if (emitType == "LargeGunItem")
	{
		auto item = GetManager().lock()->AddObjectFromCereal("LargeGunItem");
		item.lock()->transform->SetLocalPosition(gameObject.lock()->transform->GetWorldPosition());
		AddObjectFromCereal("SphereEffect", ObjectFactory::Create<Transform>(gameObject.lock()->transform->GetWorldPosition()));
	}
	else if (emitType == "ShotGunItem")
	{
		auto item = GetManager().lock()->AddObjectFromCereal("ShotGunItem");
		item.lock()->transform->SetLocalPosition(gameObject.lock()->transform->GetWorldPosition());
		AddObjectFromCereal("SphereEffect", ObjectFactory::Create<Transform>(gameObject.lock()->transform->GetWorldPosition()));
	}
	else if (emitType == "GrenadeLauncherItem")
	{
		auto item = GetManager().lock()->AddObjectFromCereal("GrenadeLauncherItem");
		item.lock()->transform->SetLocalPosition(gameObject.lock()->transform->GetWorldPosition());
		AddObjectFromCereal("SphereEffect", ObjectFactory::Create<Transform>(gameObject.lock()->transform->GetWorldPosition()));
	}
}

bool ButiEngine::BaseEnemy::ExistInHitComponent(Value_weak_ptr<GameComponent> arg_vwp_hitComponent)
{
	for (auto hitComponent : m_vec_vwp_hitComponent)
	{
		if (hitComponent == arg_vwp_hitComponent)
		{
			return true;
		}
	}

	return false;
}

void ButiEngine::BaseEnemy::CheckHasDamageInPreviousFrame()
{
	if (m_vec_vwp_hitComponent.empty())
	{
		return;
	}

	m_isInvincible = true;
	m_vlp_invincibleTime->Reset();
	m_vec_vwp_hitComponent.clear();
}

void ButiEngine::BaseEnemy::SetLookAtParameter()
{
	m_vwp_lookAt = m_vwp_drawObject.lock()->GetGameComponent<LookAtComponent>();
	auto drawObjectTransform = m_vwp_drawObject.lock()->transform;
	m_vwp_lookAt.lock()->SetLookTarget(ObjectFactory::Create<Transform>(drawObjectTransform->GetLocalPosition()));
	m_vwp_lookAt.lock()->GetLookTarget()->Translate(drawObjectTransform->GetFront() * 100.0f);
	m_vwp_lookAt.lock()->SetSpeed(0.3f);
}
