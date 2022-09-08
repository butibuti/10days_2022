#include "stdafx_u.h"
#include "EquipGun.h"
#include "SeparateDrawObject.h"
#include "Gun.h"

void ButiEngine::EquipGun::OnUpdate()
{
}

void ButiEngine::EquipGun::OnSet()
{
}

void ButiEngine::EquipGun::Start()
{
	CreateGun();
}

void ButiEngine::EquipGun::OnRemove()
{
}

void ButiEngine::EquipGun::OnShowUI()
{
	GUI::Text(u8"èeÇÃñºëO:" + m_equipGunName);

	static char gunNameBuff[128];
	GUI::InputTextWithHint("##bulletName", m_equipGunName, gunNameBuff, sizeof(gunNameBuff));
	if (GUI::Button("Set"))
	{
		m_equipGunName = gunNameBuff;
		memset(gunNameBuff, 0, 128);
	}
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::EquipGun::Clone()
{
	auto output = ObjectFactory::Create<EquipGun>();
	output->m_equipGunName = m_equipGunName;
	return output;
}

void ButiEngine::EquipGun::Dead()
{
	m_vwp_gun.lock()->SetIsRemove(true);
}

ButiEngine::Value_weak_ptr<ButiEngine::GameObject> ButiEngine::EquipGun::ChangeGun(const std::string& arg_gunName)
{
	m_equipGunName = arg_gunName;
	if (m_vwp_gun.lock())
	{
		m_vwp_gun.lock()->SetIsRemove(true);
	}

	CreateGun();
	return m_vwp_gun;
}

void ButiEngine::EquipGun::CreateGun()
{
	//GunçÏê¨
	m_vwp_gun = GetManager().lock()->AddObjectFromCereal(m_equipGunName);
	auto drawObject = gameObject.lock()->GetGameComponent<SeparateDrawObject>()->GetDrawObject();
	m_vwp_gun.lock()->transform->SetBaseTransform(drawObject.lock()->transform, true);

	m_vwp_gun.lock()->GetGameComponent<Gun>()->SetOwner(gameObject);
}
