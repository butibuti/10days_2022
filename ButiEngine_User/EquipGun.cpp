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
	//Gunì¬
	std::string gunName = "Gun_" + gameObject.lock()->GetGameObjectTags()[0].GetID();

	m_vwp_gun = GetManager().lock()->AddObjectFromCereal(gunName);
	m_vwp_gun.lock()->SetObjectName("Gun_" + gameObject.lock()->GetGameObjectName());
	m_vwp_gun.lock()->transform->SetBaseTransform(gameObject.lock()->transform, true);
}

void ButiEngine::EquipGun::OnRemove()
{
}

void ButiEngine::EquipGun::OnShowUI()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::EquipGun::Clone()
{
	return ObjectFactory::Create<EquipGun>();
}

void ButiEngine::EquipGun::Dead()
{
	m_vwp_gun.lock()->SetIsRemove(true);
}
