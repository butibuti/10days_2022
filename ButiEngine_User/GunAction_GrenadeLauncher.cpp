#include "stdafx_u.h"
#include "GunAction_GrenadeLauncher.h"

void ButiEngine::GunAction_GrenadeLauncher::OnUpdate()
{
}

void ButiEngine::GunAction_GrenadeLauncher::OnSet()
{
}

void ButiEngine::GunAction_GrenadeLauncher::Start()
{
}

void ButiEngine::GunAction_GrenadeLauncher::OnRemove()
{
}

void ButiEngine::GunAction_GrenadeLauncher::OnShowUI()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::GunAction_GrenadeLauncher::Clone()
{
	return ObjectFactory::Create<GunAction_GrenadeLauncher>();
}
