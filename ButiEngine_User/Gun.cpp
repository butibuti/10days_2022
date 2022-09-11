#include "stdafx_u.h"
#include "Gun.h"
#include "InputManager.h"
#include "Bullet.h"
#include "Bullet_GrenadeLauncher.h"

void ButiEngine::Gun::OnUpdate()
{
	if (m_vlp_shootIntervalTimer->Update())
	{
		if (!m_isShoot)
		{
			m_vlp_shootIntervalTimer->Reset();
			m_vlp_shootIntervalTimer->Stop();
			return;
		}
		Shoot();
	}
}

void ButiEngine::Gun::OnSet()
{
	m_vlp_shootIntervalTimer = ObjectFactory::Create<RelativeTimer>(m_shootIntervalFrame + 1);

	m_isShoot = false;
}

void ButiEngine::Gun::Start()
{
}

void ButiEngine::Gun::OnRemove()
{
}

void ButiEngine::Gun::OnShowUI()
{
	GUI::BulletText(u8"ägéU");
	GUI::DragFloat("##diffusion", &m_diffusion, 1.0f, 0.0f, 180.0f);

	GUI::BulletText(u8"éÀíˆ");
	GUI::DragFloat("##range", &m_range, 1.0f, 0.0f, 100.0f);

	GUI::BulletText(u8"à–óÕ");
	GUI::DragFloat("##power", &m_power, 1.0f, 0.0f, 100.0f);

	GUI::BulletText(u8"òAéÀä‘äu");
	if (GUI::DragInt("##shootIntervalFrame", &m_shootIntervalFrame, 1.0f, 0.0f, 100.0f))
	{
		if (m_vlp_shootIntervalTimer)
		{
			m_vlp_shootIntervalTimer->ChangeCountFrame(m_shootIntervalFrame + 1);
		}
	}

	GUI::Text(u8"íeÇÃñºëO:" + m_bulletName);

	static char bulletNameBuff[128];
	GUI::InputTextWithHint("##bulletName", m_bulletName, bulletNameBuff, sizeof(bulletNameBuff));
	if (GUI::Button("Set"))
	{
		m_bulletName = bulletNameBuff;
		memset(bulletNameBuff, 0, 128);
	}

	GUI::BulletText(u8"íeë¨");
	GUI::DragFloat("##bulletSpeed", &m_bulletSpeed, 0.1f, 0.0f, 1000.0f);

	GUI::BulletText(u8"àÍâÒÇ…î≠éÀÇ≥ÇÍÇÈíeÇÃêî");
	GUI::DragInt("##bulletCount", &m_bulletCount, 1.0f, 0.0f, 100.0f);

	GUI::BulletText(u8"íeÇ™ê∂ê¨Ç≥ÇÍÇÈà íu");
	GUI::DragFloat3("##offset", &m_offset.x, 0.1f, -100.0f, 100.0f);

	if (GUI::TreeNode("RecoilTransform"))
	{
		if (m_vlp_recoilTransform)
		{
			GUI::Edit(*m_vlp_recoilTransform);
			if (GUI::Button("Remove"))
			{
				m_vlp_recoilTransform = nullptr;
			}
		}
		else if (GUI::Button("AddRecoilTransform"))
		{
			m_vlp_recoilTransform = gameObject.lock()->transform->Clone();
			m_vlp_recoilTransform->SetBaseTransform(nullptr, true);
		}
		GUI::TreePop();
	}
	
	if (GUI::Button("ShootStart"))
	{
		ShootStart();
	}
	GUI::SameLine();
	if (GUI::Button("ShootStop"))
	{
		ShootStop();
	}

	if (GUI::Button("OneShoot"))
	{
		Shoot();
	}
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Gun::Clone()
{
	auto output = ObjectFactory::Create<Gun>();
	output->m_diffusion = m_diffusion;
	output->m_range = m_range;
	output->m_power = m_power;
	output->m_shootIntervalFrame = m_shootIntervalFrame;
	output->m_bulletName = m_bulletName;
	output->m_bulletSpeed = m_bulletSpeed;
	output->m_bulletCount = m_bulletCount;
	output->m_offset = m_offset;
	output->m_vlp_recoilTransform = m_vlp_recoilTransform;
	return output;
}

void ButiEngine::Gun::ShootStart()
{
	if (m_isShoot) { return; }

	m_isShoot = true;
	if (!m_vlp_shootIntervalTimer->IsOn())
	{
		Shoot();
		m_vlp_shootIntervalTimer->Start();
	}
}

void ButiEngine::Gun::ShootStop()
{
	if (!m_isShoot) { return; }

	m_isShoot = false;
}

void ButiEngine::Gun::Shoot()
{
	if (!m_isShoot) { return; }

	for (std::int32_t i = 0; i < m_bulletCount; i++)
	{
		auto bullet = GetManager().lock()->AddObjectFromCereal(m_bulletName);
		
		//îÚÇÒÇ≈Ç¢Ç≠ï˚å¸ê›íË
		bullet.lock()->transform->SetLocalRotation(gameObject.lock()->transform->GetWorldRotation());
		float rotationAngleY = ButiRandom::GetRandom(-m_diffusion, m_diffusion, 10.0f);
		bullet.lock()->transform->RollLocalRotationY_Degrees(rotationAngleY);

		//ê∂ê¨à íuê›íË
		bullet.lock()->transform->SetLocalPosition(gameObject.lock()->transform->GetWorldPosition());
		bullet.lock()->transform->Translate(m_offset * gameObject.lock()->transform->GetWorldRotation());
		bullet.lock()->transform->Translate(bullet.lock()->transform->GetFront() * bullet.lock()->transform->GetLocalScale().z * 0.5f);

		//íeÇÃèÓïÒê›íË
		SetNormalBulletParameter(bullet);
		SetGrenadeLauncherBulletParameter(bullet);
	}

	Recoil();
}

void ButiEngine::Gun::SetNormalBulletParameter(Value_weak_ptr<GameObject> arg_bullet)
{
	auto bulletComponent = arg_bullet.lock()->GetGameComponent<Bullet>();
	if (!bulletComponent) { return; }

	bulletComponent->SetOwner(m_vwp_owner);
	bulletComponent->SetPower(m_power);
	bulletComponent->SetRange(m_range);
	Vector3 velocity = arg_bullet.lock()->transform->GetFront();
	bulletComponent->SetVelocity(velocity * m_bulletSpeed);
}

void ButiEngine::Gun::SetGrenadeLauncherBulletParameter(Value_weak_ptr<GameObject> arg_bullet)
{
	auto bulletComponent = arg_bullet.lock()->GetGameComponent<Bullet_GrenadeLauncher>();
	if (!bulletComponent) { return; }

	Vector3 velocity = arg_bullet.lock()->transform->GetFront();
	bulletComponent->SetVelocity(velocity * m_bulletSpeed);
}

void ButiEngine::Gun::Recoil()
{
	if (!m_vlp_recoilTransform) { return; }

	auto anim = gameObject.lock()->AddGameComponent<TransformAnimation>();
	anim->SetTargetTransform(m_vlp_recoilTransform);
	anim->SetSpeed(1.0f / (m_shootIntervalFrame / 1.5f));
	anim->SetEaseType(Easing::EasingType::Parabola);
}
