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
			archive(m_vec_equipGunNames);
		}

		Value_weak_ptr<GameObject> GetGun(const std::int32_t arg_index = 0) { return m_vec_vwp_guns[arg_index]; }

		void Dead();
		Value_weak_ptr<GameObject> ChangeGun(const std::string& arg_gunName, const std::int32_t arg_index = 0);
		Value_weak_ptr<GameObject> AddGun(const std::string& arg_gunName);
		void RemoveGun(const std::int32_t arg_index);
	private:
		Value_weak_ptr<GameObject> CreateGun(const std::int32_t arg_index = 0);

		std::vector<Value_weak_ptr<GameObject>> m_vec_vwp_guns;

		std::vector<std::string> m_vec_equipGunNames;

		bool m_isStart = false;
	};

}

BUTI_REGIST_GAMECOMPONENT(EquipGun, true);