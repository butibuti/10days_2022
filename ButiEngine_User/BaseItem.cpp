#include "stdafx_u.h"
#include "BaseItem.h"
#include "ButiBulletWrap/ButiBulletWrap/Common.h"
#include "Header/GameObjects/DefaultGameComponent/RigidBodyComponent.h"
#include "Header//GameObjects//DefaultGameComponent//TriggerComponent.h"
#include "Player.h"

void ButiEngine::BaseItem::OnUpdate()
{
}

void ButiEngine::BaseItem::OnSet()
{
	//他のオブジェクトと当たったら死ぬ
	gameObject.lock()->AddCollisionEnterReaction(
		[this](ButiBullet::ContactData& arg_other)
		{
			Value_weak_ptr<GameObject> vwp_obj = arg_other.vwp_gameObject;
			if (vwp_obj.lock() && vwp_obj.lock()->GetGameObjectName() == "Player")
			{
				if (!m_isHitInCurrentFrame)
				{
					auto playerComponent = vwp_obj.lock()->GetGameComponent<Player>();
					if (playerComponent->CanPickUpItem())
					{
						PowerUpPlayer(playerComponent);
						Dead();
						m_isHitInCurrentFrame = true;
					}
				}
			}
		}
	);
}

void ButiEngine::BaseItem::Start()
{
	m_vwp_trigger = gameObject.lock()->GetGameComponent<TriggerComponent>();
	m_isHitInCurrentFrame = false;
}

void ButiEngine::BaseItem::OnRemove()
{
}

void ButiEngine::BaseItem::OnShowUI()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::BaseItem::Clone()
{
	return ObjectFactory::Create<BaseItem>();
}

void ButiEngine::BaseItem::PowerUpPlayer(Value_weak_ptr<Player> arg_vwp_player)
{
	if (!arg_vwp_player.lock()->CanPickUpItem())
	{
		return;
	}

	arg_vwp_player.lock()->EquipAssaultRifle();
}

void ButiEngine::BaseItem::Dead()
{
	gameObject.lock()->SetIsRemove(true);
}
