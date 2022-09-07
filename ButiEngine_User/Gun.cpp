#include "stdafx_u.h"
#include "Gun.h"
#include "InputManager.h"
#include "Bullet.h"

void ButiEngine::Gun::OnUpdate()
{
	if (m_isShoot && m_vlp_shootInterval->Update())
	{
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
	GUI::BulletText(u8"拡散");
	GUI::DragFloat("##diffusion", &m_diffusion, 1.0f, 0.0f, 180.0f);

	GUI::BulletText(u8"射程");
	GUI::DragFloat("##range", &m_range, 1.0f, 0.0f, 100.0f);

	GUI::BulletText(u8"威力");
	GUI::DragFloat("##power", &m_power, 1.0f, 0.0f, 100.0f);

	GUI::BulletText(u8"連射速度");
	if (GUI::DragInt("##rate", &m_rate, 1.0f, 0.0f, 100.0f))
	{
		if (m_vlp_shootInterval)
		{
			m_vlp_shootInterval->ChangeCountFrame(m_rate);
		}
	}

	GUI::Text(u8"弾の名前:" + m_bulletName);

	static char bulletNameBuff[128];
	GUI::InputTextWithHint("##bulletName", m_bulletName, bulletNameBuff, sizeof(bulletNameBuff));
	if (GUI::Button("Set"))
	{
		m_bulletName = bulletNameBuff;
		memset(bulletNameBuff, 0, 128);
	}

	GUI::BulletText(u8"弾速");
	GUI::DragFloat("##bulletSpeed", &m_bulletSpeed, 0.1f, 0.0f, 10.0f);

	GUI::BulletText(u8"一回に発射される弾の数");
	GUI::DragInt("##bulletCount", &m_bulletCount, 1.0f, 0.0f, 100.0f);

	GUI::BulletText(u8"弾が生成される位置");
	GUI::DragFloat3("##offset", &m_offset.x, 0.1f, -100.0f, 100.0f);

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
	output->m_offset = m_offset;
	return output;
}

void ButiEngine::Gun::ShootStart()
{
	if (m_isShoot) { return; }

	Shoot();
	m_isShoot = true;
}

void ButiEngine::Gun::ShootStop()
{
	if (!m_isShoot) { return; }

	m_isShoot = false;
}

void ButiEngine::Gun::Shoot()
{
	m_vlp_shootInterval->Reset();

	for (std::int32_t i = 0; i < m_bulletCount; i++)
	{
		auto bullet = GetManager().lock()->AddObjectFromCereal(m_bulletName);

		//生成位置設定
		bullet.lock()->transform->SetLocalPosition(gameObject.lock()->transform->GetWorldPosition());
		bullet.lock()->transform->Translate(m_offset * gameObject.lock()->transform->GetWorldRotation());
		
		//飛んでいく方向設定
		//float rotationAngleX = ButiRandom::GetRandom(0.0f, 0.5f, 10.0f);
		float rotationAngleY = ButiRandom::GetRandom(-m_diffusion, m_diffusion, 10.0f);
		bullet.lock()->transform->RollLocalRotationY_Degrees(rotationAngleY);
		//bullet.lock()->transform->RollLocalRotationX_Degrees(rotationAngleX);

		//弾の情報設定
		auto bulletComponent = bullet.lock()->GetGameComponent<Bullet>();
		bulletComponent->SetPower(m_power);
		bulletComponent->SetRange(m_range);
		Vector3 velocity = bullet.lock()->transform->GetFront();
		bulletComponent->SetVelocity(velocity * m_bulletSpeed);
	}
}
