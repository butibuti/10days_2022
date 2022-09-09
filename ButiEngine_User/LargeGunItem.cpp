#include "stdafx_u.h"
#include "LargeGunItem.h"
#include "ButiBulletWrap/ButiBulletWrap/Common.h"
#include "Header/GameObjects/DefaultGameComponent/RigidBodyComponent.h"
#include "Player.h"

void ButiEngine::LargeGunItem::OnUpdate()
{
}

void ButiEngine::LargeGunItem::OnShowUI()
{
	GUI::Text("Derrived");
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::LargeGunItem::Clone()
{
	return ObjectFactory::Create<LargeGunItem>();
}

void ButiEngine::LargeGunItem::PowerUpPlayer(Value_weak_ptr<Player> arg_vwp_player)
{
	//arg_vwp_player.lock()->PowerUp("Gun_Player_AssaultRifle");
	arg_vwp_player.lock()->EquipAssaultRifle();
}

void ButiEngine::LargeGunItem::Dead()
{
	gameObject.lock()->SetIsRemove(true);
}
