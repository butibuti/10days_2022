#include "stdafx_u.h"
#include "TitleLine.h"
#include "Header/GameObjects/DefaultGameComponent/PositionAnimationComponent.h"

void ButiEngine::TitleLine::OnUpdate()
{
}

void ButiEngine::TitleLine::OnSet()
{
}

void ButiEngine::TitleLine::Start()
{
	m_targetPos = gameObject.lock()->transform->GetLocalPosition();
	m_startPos = gameObject.lock()->transform->Translate(gameObject.lock()->transform->GetUp() * -1000.0f);
}

void ButiEngine::TitleLine::OnRemove()
{
}

void ButiEngine::TitleLine::OnShowUI()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::TitleLine::Clone()
{
	return ObjectFactory::Create<TitleLine>();
}

void ButiEngine::TitleLine::Appear()
{
	AddPositionAnimation(m_targetPos, Easing::EasingType::EaseOutExpo);
}

void ButiEngine::TitleLine::Disappear()
{
	AddPositionAnimation(m_startPos, Easing::EasingType::EaseInBack);
}

void ButiEngine::TitleLine::AddPositionAnimation(const Vector3& arg_targetPos, const Easing::EasingType arg_easeType)
{
	auto anim = gameObject.lock()->AddGameComponent<PositionAnimation>();

	anim->SetInitPosition(gameObject.lock()->transform->GetLocalPosition());
	anim->SetTargetPosition(arg_targetPos);
	anim->SetSpeed(1.0f / 15);
	anim->SetEaseType(arg_easeType);
}
