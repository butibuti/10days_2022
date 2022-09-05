#include "stdafx_u.h"
#include "Player.h"
#include "InputManager.h"

void ButiEngine::Player::OnUpdate()
{
}

void ButiEngine::Player::OnSet()
{
}

void ButiEngine::Player::Start()
{
}

void ButiEngine::Player::OnRemove()
{
}

void ButiEngine::Player::OnShowUI()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Player::Clone()
{
	return ObjectFactory::Create<Player>();
}
