#include "stdafx_u.h"
#include "Bullet.h"
#include "InputManager.h"

void ButiEngine::Bullet::OnUpdate()
{
}

void ButiEngine::Bullet::OnSet()
{
}

void ButiEngine::Bullet::Start()
{
}

void ButiEngine::Bullet::OnRemove()
{
}

void ButiEngine::Bullet::OnShowUI()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Bullet::Clone()
{
	return ObjectFactory::Create<Bullet>();
}
