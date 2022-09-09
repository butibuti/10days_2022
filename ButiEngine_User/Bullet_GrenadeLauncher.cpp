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
	std::int32_t life = 90;
	m_vlp_lifeTimer = ObjectFactory::Create<RelativeTimer>(life);
	m_vlp_lifeTimer->Start();

	gameObject.lock()->GetGameComponent<RigidBodyComponent>()->GetRigidBody()->ApplyImpulse(m_velocity);
	gameObject.lock()->GetGameComponent<RigidBodyComponent>()->GetRigidBody()->ApplyTorque(m_velocity);
}

void ButiEngine::Bullet_GrenadeLauncher::OnRemove()
{
}

void ButiEngine::Bullet_GrenadeLauncher::OnShowUI()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Bullet_GrenadeLauncher::Clone()
{
	return ObjectFactory::Create<Bullet_GrenadeLauncher>();
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
		//“G‚ª”š”­o‚µ‚½‚¢‚Æ‚«‚±‚±‚ç•Ó‚¢‚¶‚é
		//explosionName = "Explosion_Enemy_Normal";
	}
	auto explosion = GetManager().lock()->AddObjectFromCereal(explosionName);
	explosion.lock()->transform->SetLocalPosition(gameObject.lock()->transform->GetLocalPosition());

	gameObject.lock()->SetIsRemove(true);
}
