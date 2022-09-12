#include "stdafx_u.h"
#include "PreAction.h"

void ButiEngine::PreAction::OnUpdate()
{
}

void ButiEngine::PreAction::OnSet()
{
}

void ButiEngine::PreAction::Start()
{
}

void ButiEngine::PreAction::OnRemove()
{
}

void ButiEngine::PreAction::OnShowUI()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::PreAction::Clone()
{
	return ObjectFactory::Create<PreAction>();
}
