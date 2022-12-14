#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Player;
	class Gun;

	class GunAction_AssaultRifle :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "GunAction_AssaultRifle";
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
		}
	private:
		void RotateStart();
		void RotateStop();
		void ShootStart();
		void ShootStop();

		std::int32_t m_phase;

		Value_ptr<RelativeTimer> m_vlp_rotateTimer;
		Value_ptr<RelativeTimer> m_vlp_shootTimer;

		Value_weak_ptr<Player> m_vwp_playerComponent;
		Value_weak_ptr<Gun> m_vwp_gunComponent;
		Value_weak_ptr<GameObject> m_vwp_drawObject;
		Value_weak_ptr<Transform> m_vwp_lookTarget;
	};

}

BUTI_REGIST_GAMECOMPONENT(GunAction_AssaultRifle, true);