#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Player;
	class Gun;

	enum class GunAction_GrenadeLauncherPhase
	{
		Shoot,
		Wait,
	};

	class GunAction_GrenadeLauncher :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "GunAction_GrenadeLauncher";
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
		void StartShootPhase();
		void UpdateShootPhase();

		void StartWaitPhase();
		void UpdateWaitPhase();
		
		GunAction_GrenadeLauncherPhase m_phase;

		Value_ptr<RelativeTimer> m_vlp_shootTimer;
		float m_rotationAngle;

		Value_ptr<RelativeTimer> m_vlp_waitTimer;

		Value_weak_ptr<Player> m_vwp_playerComponent;
		Value_weak_ptr<Gun> m_vwp_gunComponent;
		Value_weak_ptr<GameObject> m_vwp_drawObject;
		Value_weak_ptr<Transform> m_vwp_lookTarget;
	};

}

BUTI_REGIST_GAMECOMPONENT(GunAction_GrenadeLauncher, true);