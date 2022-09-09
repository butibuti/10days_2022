#include "stdafx_u.h"
#include "DamageArea.h"

void ButiEngine::DamageArea::OnUpdate()
{
	if (m_vlp_lifeTimer->Update())
	{
		Dead();
	}

	m_canGiveDamage = false;
	if (m_vlp_giveDamageInterval->Update())
	{
		m_canGiveDamage = true;
	}
}

void ButiEngine::DamageArea::OnSet()
{
}

void ButiEngine::DamageArea::Start()
{
	m_vlp_lifeTimer = ObjectFactory::Create<RelativeTimer>(m_life);
	m_vlp_lifeTimer->Start();

	m_vlp_giveDamageInterval = ObjectFactory::Create<RelativeTimer>(m_giveDamageIntervalFrame + 1);
	m_vlp_giveDamageInterval->Start();

	m_canGiveDamage = true;
}

void ButiEngine::DamageArea::OnRemove()
{
}

void ButiEngine::DamageArea::OnShowUI()
{
	GUI::BulletText(u8"ライフ");
	GUI::DragInt("##life", &m_life, 1.0f, 1.0f, 100.0f);

	GUI::BulletText(u8"威力");
	GUI::DragFloat("##power", &m_power, 1.0f, 0.0f, 100.0f);

	GUI::BulletText(u8"ダメージ間隔");
	if (GUI::DragInt("##giveDamageIntervalFrame", &m_giveDamageIntervalFrame, 1.0f, 0.0f, 100.0f))
	{
		if (m_vlp_giveDamageInterval)
		{
			m_vlp_giveDamageInterval->ChangeCountFrame(m_giveDamageIntervalFrame + 1);
		}
	}
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::DamageArea::Clone()
{
	auto output = ObjectFactory::Create<DamageArea>();
	output->m_life = m_life;
	output->m_power = m_power;
	output->m_giveDamageIntervalFrame = m_giveDamageIntervalFrame;
	return output;
}

void ButiEngine::DamageArea::Dead()
{
	gameObject.lock()->SetIsRemove(true);
}