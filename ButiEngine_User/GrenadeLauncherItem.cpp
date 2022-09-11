#include "stdafx_u.h"
#include "GrenadeLauncherItem.h"
#include "ButiBulletWrap/ButiBulletWrap/Common.h"
#include "Header/GameObjects/DefaultGameComponent/RigidBodyComponent.h"
#include "Player.h"

void ButiEngine::GrenadeLauncherItem::OnUpdate()
{
}

void ButiEngine::GrenadeLauncherItem::OnShowUI()
{
	GUI::Text("Derrived");
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::GrenadeLauncherItem::Clone()
{
	return ObjectFactory::Create<GrenadeLauncherItem>();
}

void ButiEngine::GrenadeLauncherItem::PowerUpPlayer(Value_weak_ptr<Player> arg_vwp_player)
{
	if (!arg_vwp_player.lock()->CanPickUpItem())
	{
		return;
	}

	arg_vwp_player.lock()->EquipGrenadeLauncher();
}

void ButiEngine::GrenadeLauncherItem::Dead()
{
	gameObject.lock()->SetIsRemove(true);
}
