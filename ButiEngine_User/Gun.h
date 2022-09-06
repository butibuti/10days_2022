#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	enum class GunType
	{
		PlayerGun,
		SpreadGun,
		BombGun,
		LaserGun,
	};

	class Gun :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "Gun";
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
			archive(m_gunType);
		}

		void Shoot();
	private:
		void ShootPlayerGun();
		void ShootSpreadGun();
		void ShootBombGun();
		void ShootLaserGun();


		GunType m_gunType;

		std::int32_t m_gui_typeNum;
	};

}

BUTI_REGIST_GAMECOMPONENT(Gun, true);