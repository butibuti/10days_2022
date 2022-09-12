#include "stdafx_u.h"
#include "Hexagon.h"
#include "Header/GameObjects/DefaultGameComponent/PositionAnimationComponent.h"
#include "Header/GameObjects/DefaultGameComponent/ScaleAnimationComponent.h"
#include "InputManager.h"

void ButiEngine::Hexagon::OnUpdate()
{
	Vector3 pos = gameObject.lock()->transform->TranslateX(m_moveSpeed);
	Vector3 scale = gameObject.lock()->transform->GetLocalScale();
	if (pos.x >= 1300.0f + scale.x * 0.5f)
	{
		gameObject.lock()->transform->SetLocalPositionX(-1200.0f - scale.x * 0.5f);
	}
	else if (pos.x <= -1300.0f - scale.x * 0.5f)
	{
		gameObject.lock()->transform->SetLocalPositionX(1200.0f + scale.x * 0.5f);
	}

	gameObject.lock()->transform->RollLocalRotationZ_Degrees(m_rotationAngle);
}

void ButiEngine::Hexagon::OnSet()
{
}

void ButiEngine::Hexagon::Start()
{
	m_moveSpeed = ButiRandom::GetRandom(2.0f, 2.5f, 10) * (ButiRandom::GetInt(0, 1) ? 1.0f : -1.0f);
	m_rotationAngle = 0.3f * (ButiRandom::GetInt(0, 1) ? 1.0f : -1.0f);
}

void ButiEngine::Hexagon::OnRemove()
{
}

void ButiEngine::Hexagon::OnShowUI()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Hexagon::Clone()
{
	return ObjectFactory::Create<Hexagon>();
}

void ButiEngine::Hexagon::Appear()
{
	m_startScale = gameObject.lock()->transform->GetLocalScale();
	gameObject.lock()->transform->SetLocalScale(Vector3(0, 0, 1));

	AddScaleAnimation();
}

void ButiEngine::Hexagon::Disappear()
{
	AddPositionAnimation();
}

void ButiEngine::Hexagon::AddPositionAnimation()
{
	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();
	Vector3 targetPos = Vector3(-2000.0f, -1000.0f, 0.0f);
	targetPos *= (pos.x < 0.0f) ? 1.0f : -1.0f;

	auto anim = gameObject.lock()->AddGameComponent<PositionAnimation>();
	anim->SetInitPosition(pos);
	anim->SetTargetPosition(targetPos);
	anim->SetSpeed(1.0f / 20);
	anim->SetEaseType(Easing::EasingType::EaseInBack);
}

void ButiEngine::Hexagon::AddScaleAnimation()
{
	auto anim = gameObject.lock()->AddGameComponent<ScaleAnimation>();
	anim->SetInitScale(gameObject.lock()->transform->GetLocalScale());
	anim->SetTargetScale(m_startScale);
	anim->SetSpeed(1.0f / ButiRandom::GetInt(30, 50));
	anim->SetEaseType(Easing::EasingType::EaseOutCirc);
}
