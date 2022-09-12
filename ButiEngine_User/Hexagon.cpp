#include "stdafx_u.h"
#include "Hexagon.h"
#include "Header/GameObjects/DefaultGameComponent/PositionAnimationComponent.h"
#include "InputManager.h"

void ButiEngine::Hexagon::OnUpdate()
{
	if (!m_isDisappear && InputManager::IsTriggerDecideKey())
	{
		m_isDisappear = true;
		AddPositionAnimation();
	}

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
	m_isDisappear = false;
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
