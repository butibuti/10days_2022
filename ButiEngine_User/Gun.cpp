#include "stdafx_u.h"
#include "Gun.h"
#include "InputManager.h"

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
	GUI::DragFloat("##diffusion", &m_diffusion, 1.0f, 0.0f, 100.0f);

	GUI::BulletText(u8"射程");
	GUI::DragFloat("##range", &m_range, 1.0f, 0.0f, 100.0f);

	GUI::BulletText(u8"威力");
	GUI::DragFloat("##power", &m_power, 1.0f, 0.0f, 100.0f);

	GUI::BulletText(u8"連射速度");
	if (GUI::DragInt("##rate", &m_rate, 1.0f, 0.0f, 100.0f))
	{
		m_vlp_shootInterval->ChangeCountFrame(m_rate);
	}

	GUI::BulletText(u8"弾速");
	GUI::DragFloat("##bulletSpeed", &m_bulletSpeed, 1.0f, 0.0f, 100.0f);

	GUI::BulletText(u8"一回に発射される弾の数");
	GUI::DragInt("##bulletCount", &m_bulletCount, 1.0f, 0.0f, 100.0f);

	GUI::BulletText(u8"弾が生成される位置");
	GUI::DragFloat3("##offset", &m_offset.x, 0.1f, -100.0f, 100.0f);

	if (GUI::Button("ShootStart"))
	{
		SetIsShoot(true);
	}
	GUI::SameLine();
	if (GUI::Button("ShootStop"))
	{
		SetIsShoot(false);
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
	output->m_bulletSpeed = m_bulletSpeed;
	output->m_offset = m_offset;
	return output;
}

void ButiEngine::Gun::SetIsShoot(const bool arg_isShoot)
{
	//isShootがtrueになったフレームに一発撃つ
	if (arg_isShoot && !m_isShoot)
	{
		Shoot();
	}

	m_isShoot = arg_isShoot;
}

void ButiEngine::Gun::Shoot()
{
	m_vlp_shootInterval->Reset();
}
