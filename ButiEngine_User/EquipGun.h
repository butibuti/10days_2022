#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {
	class EquipGun :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "EquipGun";
		}
		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		void OnRemove()override;
		void OnShowUI()override;
		Value_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
			archive(m_equipGunName);
		}

		Value_weak_ptr<GameObject> GetGun() { return m_vwp_gun; }

		void Dead();
	private:
		Value_weak_ptr<GameObject> m_vwp_gun;

		std::string m_equipGunName = "Gun_Player_Normal";
	};

}

BUTI_REGIST_GAMECOMPONENT(EquipGun, true);