#include "stdafx_u.h"
#include "Bullet_GrenadeLauncher.h"
#include "Header/GameObjects/DefaultGameComponent/RigidBodyComponent.h"

void ButiEngine::Bullet_GrenadeLauncher::OnUpdate()
{
	if (m_vlp_lifeTimer->Update())
	{
		Dead();
	}
}

void ButiEngine::Bullet_GrenadeLauncher::OnSet()
{
}

void ButiEngine::Bullet_GrenadeLauncher::Start()
{
	m_vlp_lifeTimer = ObjectFactory::Create<RelativeTimer>(m_life);
	m_vlp_lifeTimer->Start();

	gameObject.lock()->GetGameComponent<RigidBodyComponent>()->GetRigidBody()->ApplyImpulse(m_velocity);
}

void ButiEngine::Bullet_GrenadeLauncher::OnRemove()
{
}

void ButiEngine::Bullet_GrenadeLauncher::OnShowUI()
{
	GUI::BulletText("Life");
	if (GUI::DragInt("##life", &m_life, 1.0f, 1.0f, 1000.0f))
	{
		if (m_vlp_lifeTimer)
		{
			m_vlp_lifeTimer->ChangeCountFrame(m_life);
		}
	}
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Bullet_GrenadeLauncher::Clone()
{
	auto output = ObjectFactory::Create<Bullet_GrenadeLauncher>();
	output->m_life = m_life;
	return output;
}

void ButiEngine::Bullet_GrenadeLauncher::Dead()
{
	std::string explosionName;
	if (gameObject.lock()->HasGameObjectTag("Bullet_Player"))
	{
		explosionName = "Explosion_Player_Normal";
	}
	else if (gameObject.lock()->HasGameObjectTag("Bullet_Enemy"))
	{
		//敵が爆発出したいときここら辺いじる
		//explosionName = "Explosion_Enemy_Normal";
	}
	auto explosion = GetManager().lock()->AddObjectFromCereal(explosionName);
	explosion.lock()->transform->SetLocalPosition(gameObject.lock()->transform->GetLocalPosition());

	gameObject.lock()->SetIsRemove(true);
}
