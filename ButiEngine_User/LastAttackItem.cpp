#include "stdafx_u.h"
#include "LastAttackItem.h"
#include "ButiBulletWrap/ButiBulletWrap/Common.h"
#include "Header/GameObjects/DefaultGameComponent/RigidBodyComponent.h"
#include "Player.h"

void ButiEngine::LastAttackItem::OnUpdate()
{
}

void ButiEngine::LastAttackItem::OnShowUI()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::LastAttackItem::Clone()
{
	return ObjectFactory::Create<LastAttackItem>();
}

void ButiEngine::LastAttackItem::PowerUpPlayer(Value_weak_ptr<Player> arg_vwp_player)
{
	if (!arg_vwp_player.lock()->CanPickUpItem())
	{
		return;
	}

	arg_vwp_player.lock()->EquipLastAttackGun();
}

void ButiEngine::LastAttackItem::Dead()
{
	gameObject.lock()->SetIsRemove(true);
}
