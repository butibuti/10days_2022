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
	GUI::BulletText(u8"�g�U");
	GUI::DragFloat("##diffusion", &m_diffusion, 1.0f, 0.0f, 100.0f);

	GUI::BulletText(u8"�˒�");
	GUI::DragFloat("##range", &m_range, 1.0f, 0.0f, 100.0f);

	GUI::BulletText(u8"�З�");
	GUI::DragFloat("##power", &m_power, 1.0f, 0.0f, 100.0f);

	GUI::BulletText(u8"�A�ˑ��x");
	if (GUI::DragInt("##rate", &m_rate, 1.0f, 0.0f, 100.0f))
	{
		m_vlp_shootInterval->ChangeCountFrame(m_rate);
	}

	GUI::BulletText(u8"�e��");
	GUI::DragFloat("##bulletSpeed", &m_bulletSpeed, 1.0f, 0.0f, 1000.0f);

	GUI::BulletText(u8"���ɔ��˂����e�̐�");
	GUI::DragInt("##bulletCount", &m_bulletCount, 1.0f, 0.0f, 100.0f);

	GUI::BulletText(u8"�e�����������ʒu");
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
	//isShoot��true�ɂȂ����t���[���Ɉꔭ����
	if (arg_isShoot && !m_isShoot)
	{
		Shoot();
	}

	m_isShoot = arg_isShoot;
}

void ButiEngine::Gun::Shoot()
{
	m_vlp_shootInterval->Reset();

	for (std::int32_t i = 0; i < m_bulletCount; i++)
	{
		auto bulletTransform = gameObject.lock()->transform->Clone();

		bulletTransform->Translate(m_offset * gameObject.lock()->transform->GetWorldRotation());

		bulletTransform->SetLocalScale(0.35f);

		float rotationAngleX = ButiRandom::GetRandom(-0.5f, 0.5f, 10.0f);
		float rotationAngleY = ButiRandom::GetRandom(-m_diffusion, m_diffusion, 10.0f);
		bulletTransform->RollLocalRotationY_Degrees(rotationAngleY);
		bulletTransform->RollLocalRotationX_Degrees(rotationAngleX);

		Vector3 velocity = bulletTransform->GetFront();
		
		auto bullet = GetManager().lock()->AddObjectFromCereal("Bullet", bulletTransform);
		auto bulletComponent = bullet.lock()->GetGameComponent<Bullet>();
		bulletComponent->SetPower(m_power);
		bulletComponent->SetRange(m_range);
		bulletComponent->SetVelocity(velocity * m_bulletSpeed * GameDevice::WorldSpeed);
	}


}