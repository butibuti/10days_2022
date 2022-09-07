#include "stdafx_u.h"
#include "EquipGun.h"

void ButiEngine::EquipGun::OnUpdate()
{
}

void ButiEngine::EquipGun::OnSet()
{
}

void ButiEngine::EquipGun::Start()
{
	//GunçÏê¨
	m_vwp_gun = GetManager().lock()->AddObjectFromCereal(m_equipGunName);
	m_vwp_gun.lock()->SetObjectName("Gun_" + gameObject.lock()->GetGameObjectName());
	m_vwp_gun.lock()->transform->SetBaseTransform(gameObject.lock()->transform, true);
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
