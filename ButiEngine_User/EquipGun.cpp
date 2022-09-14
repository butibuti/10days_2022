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
	m_isStart = true;

	std::int32_t gunCount = m_vec_equipGunNames.size();
	m_vec_vwp_guns.resize(gunCount);
	for (std::int32_t i = 0; i < gunCount; i++)
	{
		CreateGun(i);
	}
}

void ButiEngine::EquipGun::OnRemove()
{
}

void ButiEngine::EquipGun::OnShowUI()
{
	if (m_isStart) { return; }

	if (GUI::Button("Add"))
	{
		m_vec_equipGunNames.push_back("");
	}

	std::int32_t gunCount = m_vec_equipGunNames.size();
	for (std::int32_t i = 0; i < gunCount; i++)
	{
		if (GUI::TreeNode("Gun_" + std::to_string(i)))
		{
			if (GUI::Button("Remove"))
			{
				m_vec_equipGunNames.erase(m_vec_equipGunNames.begin() + i);
				GUI::TreePop();
				break;
			}

			GUI::Text(u8"e‚Ì–¼‘O[" + std::to_string(i) + "]:" + m_vec_equipGunNames[i]);

			static char gunNameBuff[5][128];
			GUI::InputTextWithHint("##gunName" + std::to_string(i), m_vec_equipGunNames[i], gunNameBuff[i], sizeof(gunNameBuff[i]));
			if (GUI::Button("Set"))
			{
				m_vec_equipGunNames[i] = gunNameBuff[i];
				memset(gunNameBuff[i], 0, 128);
			}
			GUI::TreePop();
		}
	}
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::EquipGun::Clone()
{
	auto output = ObjectFactory::Create<EquipGun>();
	output->m_vec_equipGunNames = m_vec_equipGunNames;
	return output;
}

void ButiEngine::EquipGun::Dead()
{
	auto end = m_vec_vwp_guns.end();
	for (auto itr = m_vec_vwp_guns.begin(); itr != end; ++itr)
	{
		(*itr).lock()->SetIsRemove(true);
	}
	gameObject.lock()->SetIsRemove(true);
}

ButiEngine::Value_weak_ptr<ButiEngine::GameObject> ButiEngine::EquipGun::ChangeGun(const std::string& arg_gunName, const std::int32_t arg_index, const bool arg_isThrow)
{
	m_vec_equipGunNames[arg_index] = arg_gunName;
	if (m_vec_vwp_guns[arg_index].lock())
	{
		if (arg_isThrow)
		{
			m_vec_vwp_guns[arg_index].lock()->GetGameComponent<Gun>()->Dead();
		}
		else
		{
			m_vec_vwp_guns[arg_index].lock()->SetIsRemove(true);
		}
	}

	return CreateGun(arg_index);
}

ButiEngine::Value_weak_ptr<ButiEngine::GameObject> ButiEngine::EquipGun::AddGun(const std::string& arg_gunName)
{
	m_vec_equipGunNames.push_back(arg_gunName);

	m_vec_vwp_guns.resize(m_vec_vwp_guns.size() + 1);
	return CreateGun(m_vec_vwp_guns.size() - 1);
}

void ButiEngine::EquipGun::RemoveGun(const std::int32_t arg_index, const bool arg_isThrow)
{
	m_vec_equipGunNames.erase(m_vec_equipGunNames.begin() + arg_index);

	if (arg_isThrow)
	{
		m_vec_vwp_guns[arg_index].lock()->GetGameComponent<Gun>()->Dead();
	}
	else
	{
		m_vec_vwp_guns[arg_index].lock()->SetIsRemove(true);
	}
	m_vec_vwp_guns.erase(m_vec_vwp_guns.begin() + arg_index);
}

ButiEngine::Value_weak_ptr<ButiEngine::GameObject> ButiEngine::EquipGun::CreateGun(const std::int32_t arg_index)
{
	if (m_vec_equipGunNames[arg_index].empty())
	{
		auto gun = Value_weak_ptr<GameObject>();
		m_vec_vwp_guns[arg_index] = gun;
		return gun;
	}

	auto gun = GetManager().lock()->AddObjectFromCereal(m_vec_equipGunNames[arg_index]);
	auto drawObject = gameObject.lock()->GetGameComponent<SeparateDrawObject>()->GetDrawObject();
	gun.lock()->transform->SetBaseTransform(drawObject.lock()->transform, true);
	gun.lock()->GetGameComponent<Gun>()->SetOwner(gameObject);

	m_vec_vwp_guns[arg_index] = gun;

	return gun;
}
