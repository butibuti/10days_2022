#include "stdafx_u.h"
#include "ShotGunItem.h"
#include "ButiBulletWrap/ButiBulletWrap/Common.h"
#include "Header/GameObjects/DefaultGameComponent/RigidBodyComponent.h"
#include "Player.h"

void ButiEngine::ShotGunItem::OnUpdate()
{
}

void ButiEngine::ShotGunItem::OnShowUI()
{
	GUI::Text("Derrived");
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::ShotGunItem::Clone()
{
	return ObjectFactory::Create<ShotGunItem>();
}

void ButiEngine::ShotGunItem::PowerUpPlayer(Value_weak_ptr<Player> arg_vwp_player)
{
	if (!arg_vwp_player.lock()->CanPickUpItem())
	{
		return;
	}

	arg_vwp_player.lock()->EquipShotgun();
}

void ButiEngine::ShotGunItem::Dead()
{
	gameObject.lock()->SetIsRemove(true);
}
