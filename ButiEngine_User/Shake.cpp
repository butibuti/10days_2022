#include "stdafx_u.h"
#include "Shake.h"

void ButiEngine::Shake::OnUpdate()
{
	if (m_vlp_shakeIntervalTimer->Update())
	{
		Move();
	}

	if (m_vlp_shakeTimer->Update())
	{
		Dead();
	}
}

void ButiEngine::Shake::OnSet()
{
	m_startPos = gameObject.lock()->transform->GetLocalPosition();
	m_vlp_shakeTimer = ObjectFactory::Create<RelativeTimer>(m_shakeFrame);
	m_vlp_shakeTimer->Start();
	m_vlp_shakeIntervalTimer = ObjectFactory::Create<RelativeTimer>(m_shakeIntervalFrame);
	m_vlp_shakeIntervalTimer->Start();
}

void ButiEngine::Shake::Start()
{
}

void ButiEngine::Shake::OnRemove()
{
}

void ButiEngine::Shake::OnShowUI()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Shake::Clone()
{
	return ObjectFactory::Create<Shake>();
}

void ButiEngine::Shake::Dead()
{
	gameObject.lock()->transform->SetLocalPosition(m_startPos);
	SetIsRemove(true);
}

void ButiEngine::Shake::Move()
{
	Vector3 targetPos = m_startPos;
	targetPos += gameObject.lock()->transform->GetRight() * ButiRandom::GetRandom(-m_amplitude.x * 0.5f, m_amplitude.x * 0.5f, 100);
	targetPos += gameObject.lock()->transform->GetUp() * ButiRandom::GetRandom(-m_amplitude.y * 0.5f, m_amplitude.y * 0.5f, 100);
	targetPos += gameObject.lock()->transform->GetFront() * ButiRandom::GetRandom(-m_amplitude.z * 0.5f, m_amplitude.z * 0.5f, 100);

	gameObject.lock()->transform->SetLocalPosition(targetPos);
}
