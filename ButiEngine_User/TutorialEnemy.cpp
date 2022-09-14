#include "stdafx_u.h"
#include "TutorialEnemy.h"
#include "InputManager.h"
#include "Header/GameObjects/DefaultGameComponent/RigidBodyComponent.h"
#include "SeparateDrawObject.h"
#include "Gun.h"
#include "EquipGun.h"
#include "Player.h"
#include "Bullet.h"
#include "DamageArea.h"
#include "ItemEmitParameter.h"

void ButiEngine::TutorialEnemy::OnUpdate()
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

void ButiEngine::TutorialEnemy::OnSet()
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

void ButiEngine::TutorialEnemy::Start()
{
	m_vwp_rigidBody = gameObject.lock()->GetGameComponent<RigidBodyComponent>();
	m_vwp_drawObject = gameObject.lock()->GetGameComponent<SeparateDrawObject>()->GetDrawObject().lock();
	m_vwp_drawObject.lock()->transform->SetLocalRotation(Vector3());
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
	m_direction = Vector3();
	m_isPause = false;
}

void ButiEngine::TutorialEnemy::OnShowUI()
{
	BaseEnemy::OnShowUI();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::TutorialEnemy::Clone()
{
	auto output = ObjectFactory::Create<TutorialEnemy>();
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

void ButiEngine::TutorialEnemy::Dead()
{
	EmitItem();
	gameObject.lock()->GetGameComponent<SeparateDrawObject>()->Dead();
	m_vwp_equipGunComponent.lock()->Dead();
	gameObject.lock()->SetIsRemove(true);
}

void ButiEngine::TutorialEnemy::Control()
{
	//直前フレームにダメージがあったかを判定
	CheckHasDamageInPreviousFrame();

	Attack();
}

void ButiEngine::TutorialEnemy::Attack()
{
	m_vwp_gunComponent.lock()->ShootStart();
}

void ButiEngine::TutorialEnemy::Damage(const int32_t arg_power)
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

void ButiEngine::TutorialEnemy::EmitItem()
{
	Value_weak_ptr<ItemEmitParameter> vwp_itemEmitParameterComponent = gameObject.lock()->GetGameComponent<ItemEmitParameter>();
	if (!vwp_itemEmitParameterComponent.lock())
	{
		return;
	}
	std::string emitType = vwp_itemEmitParameterComponent.lock()->CalculateItemEmitType();

	if (emitType == "LargeGunItem")
	{
		auto item = GetManager().lock()->AddObjectFromCereal("LargeGunItem");
		item.lock()->transform->SetLocalPosition(gameObject.lock()->transform->GetWorldPosition());
		AddObjectFromCereal("SphereEffect", ObjectFactory::Create<Transform>(gameObject.lock()->transform->GetWorldPosition()));
	}
}
