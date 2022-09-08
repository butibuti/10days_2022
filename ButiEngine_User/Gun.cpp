#include "stdafx_u.h"
#include "Gun.h"
#include "InputManager.h"
#include "Bullet.h"

void ButiEngine::Gun::OnUpdate()
{
	if (m_vlp_shootInterval->Update())
	{
		if (!m_isShoot)
		{
			m_vlp_shootInterval->Reset();
			m_vlp_shootInterval->Stop();
			return;
		}
		Shoot();
	}
}

void ButiEngine::Gun::OnSet()
{
}

void ButiEngine::Gun::Start()
{
	m_vlp_shootInterval = ObjectFactory::Create<RelativeTimer>(m_rate);

	m_vlp_shootInterval->Start();

	m_isShoot = false;
}

void ButiEngine::Gun::OnRemove()
{
}

void ButiEngine::Gun::OnShowUI()
{
	GUI::BulletText(u8"ŠgŽU");
	GUI::DragFloat("##diffusion", &m_diffusion, 1.0f, 0.0f, 180.0f);

	GUI::BulletText(u8"ŽË’ö");
	GUI::DragFloat("##range", &m_range, 1.0f, 0.0f, 100.0f);

	GUI::BulletText(u8"ˆÐ—Í");
	GUI::DragFloat("##power", &m_power, 1.0f, 0.0f, 100.0f);

	GUI::BulletText(u8"˜AŽË‘¬“x");
	if (GUI::DragInt("##rate", &m_rate, 1.0f, 0.0f, 100.0f))
	{
		if (m_vlp_shootInterval)
		{
			m_vlp_shootInterval->ChangeCountFrame(m_rate);
		}
	}

	GUI::Text(u8"’e‚Ì–¼‘O:" + m_bulletName);

	static char bulletNameBuff[128];
	GUI::InputTextWithHint("##bulletName", m_bulletName, bulletNameBuff, sizeof(bulletNameBuff));
	if (GUI::Button("Set"))
	{
		m_bulletName = bulletNameBuff;
		memset(bulletNameBuff, 0, 128);
	}

	GUI::BulletText(u8"’e‘¬");
	GUI::DragFloat("##bulletSpeed", &m_bulletSpeed, 0.1f, 0.0f, 10.0f);

	GUI::BulletText(u8"ˆê‰ñ‚É”­ŽË‚³‚ê‚é’e‚Ì”");
	GUI::DragInt("##bulletCount", &m_bulletCount, 1.0f, 0.0f, 100.0f);

	GUI::BulletText(u8"’e‚ª¶¬‚³‚ê‚éˆÊ’u");
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
	output->m_rate = m_rate;
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
	if (!m_vlp_shootInterval->IsOn())
	{
		Shoot();
		m_vlp_shootInterval->Start();
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

		//¶¬ˆÊ’uÝ’è
		bullet.lock()->transform->SetLocalPosition(gameObject.lock()->transform->GetWorldPosition());
		bullet.lock()->transform->Translate(m_offset * gameObject.lock()->transform->GetWorldRotation());
		
		//”ò‚ñ‚Å‚¢‚­•ûŒüÝ’è
		bullet.lock()->transform->SetLocalRotation(gameObject.lock()->transform->GetWorldRotation());
		float rotationAngleY = ButiRandom::GetRandom(-m_diffusion, m_diffusion, 10.0f);
		bullet.lock()->transform->RollLocalRotationY_Degrees(rotationAngleY);

		//’e‚Ìî•ñÝ’è
		auto bulletComponent = bullet.lock()->GetGameComponent<Bullet>();
		bulletComponent->SetOwner(m_vwp_owner);
		bulletComponent->SetPower(m_power);
		bulletComponent->SetRange(m_range);
		Vector3 velocity = bullet.lock()->transform->GetFront();
		bulletComponent->SetVelocity(velocity * m_bulletSpeed);
	}

	Recoil();
}

void ButiEngine::Gun::Recoil()
{
	if (!m_vlp_recoilTransform) { return; }

	auto anim = gameObject.lock()->AddGameComponent<TransformAnimation>();
	anim->SetTargetTransform(m_vlp_recoilTransform);
	anim->SetSpeed(1.0f / (m_rate / 1.5f));
	anim->SetEaseType(Easing::EasingType::Parabola);
}
