#include "TestBehavior.h"
#include"Header/Common/CollisionPrimitive.h"
float g_vibe_l = 0.0f, g_vibe_r = 0.0f;
void ButiEngine::TestBehavior::OnUpdate()
{

}


void ButiEngine::TestBehavior::OnSet()
{
}

void ButiEngine::TestBehavior::Start()
{
	

}
void ButiEngine::TestBehavior::OnRemove()
{
	
}

void ButiEngine::TestBehavior::OnShowUI()
{
}

void ButiEngine::TestBehavior::ShowGUI()
{

}

ButiEngine::Vector3 ButiEngine::TestBehavior::GetWorldPosition()
{
	return gameObject.lock()->transform->GetWorldPosition();
}

void ButiEngine::TestBehavior::AdditionMass(const float arg_mass)
{

}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::TestBehavior::Clone()
{
	auto clone = ObjectFactory::Create<TestBehavior>();
	return clone;
}