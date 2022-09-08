#include "stdafx_u.h"
#include "LergeGunItem.h"
#include "ButiBulletWrap/ButiBulletWrap/Common.h"
#include "Header/GameObjects/DefaultGameComponent/RigidBodyComponent.h"
#include "Player.h"

void ButiEngine::LergeGunItem::OnUpdate()
{
}

void ButiEngine::LergeGunItem::OnShowUI()
{
	GUI::Text("Derrived");
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::LergeGunItem::Clone()
{
	return ObjectFactory::Create<LergeGunItem>();
}

void ButiEngine::LergeGunItem::PowerUpPlayer(Value_weak_ptr<Player> arg_vwp_player)
{
	arg_vwp_player.lock()->PowerUp("Gun_Player_Lerge");
}

void ButiEngine::LergeGunItem::Dead()
{
	gameObject.lock()->SetIsRemove(true);
}
