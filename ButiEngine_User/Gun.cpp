#include "stdafx_u.h"
#include "Gun.h"
#include "InputManager.h"

void ButiEngine::Gun::OnUpdate()
{
}

void ButiEngine::Gun::OnSet()
{
}

void ButiEngine::Gun::Start()
{
}

void ButiEngine::Gun::OnRemove()
{
}

void ButiEngine::Gun::OnShowUI()
{
	GUI::RadioButton("PlayerGun", m_gui_typeNum, 0);
	GUI::RadioButton("SpreadGun", m_gui_typeNum, 1);
	GUI::RadioButton("BombGun", m_gui_typeNum, 2);
	GUI::RadioButton("LaserGun", m_gui_typeNum, 3);

	m_gunType = static_cast<GunType>(m_gui_typeNum);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Gun::Clone()
{
	auto output = ObjectFactory::Create<Gun>();
	output->m_gunType = m_gunType;
	return output;
}

void ButiEngine::Gun::Shoot()
{
	switch (m_gunType)
	{
	case GunType::PlayerGun:
		ShootPlayerGun();
		break;
	case GunType::SpreadGun:
		ShootSpreadGun();
		break;
	case GunType::BombGun:
		ShootBombGun();
		break;
	case GunType::LaserGun:
		ShootLaserGun();
		break;
	}
}

void ButiEngine::Gun::ShootPlayerGun()
{
}

void ButiEngine::Gun::ShootSpreadGun()
{
}

void ButiEngine::Gun::ShootBombGun()
{
}

void ButiEngine::Gun::ShootLaserGun()
{
}
