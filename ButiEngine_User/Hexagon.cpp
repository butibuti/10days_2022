#include "stdafx_u.h"
#include "Hexagon.h"

void ButiEngine::Hexagon::OnUpdate()
{
	Vector3 pos = gameObject.lock()->transform->TranslateX(m_moveSpeed);
	if (pos.x >= 1300.0f)
	{
		gameObject.lock()->transform->SetLocalPositionX(-1200.0f);
	}
	else if (pos.x <= -1300.0f)
	{
		gameObject.lock()->transform->SetLocalPositionX(1200.0f);
	}

	gameObject.lock()->transform->RollLocalRotationZ_Degrees(m_rotationAngle);
}

void ButiEngine::Hexagon::OnSet()
{
}

void ButiEngine::Hexagon::Start()
{
	m_moveSpeed = ButiRandom::GetRandom(-10.0f, 10.0f, 100);
	m_rotationAngle = ButiRandom::GetRandom(-3.0f, 3.0f, 100);
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
