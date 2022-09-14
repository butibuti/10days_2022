#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Player;
	class Gun;

	enum class GunAction_GrenadeLauncherPhase
	{
		MoveOffScreen,
		MoveOffScreenWait,
		Shoot,
		ShootWait,
		ReturnCenter,
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
		void StartMoveOffScreenPhase();
		void UpdateMoveOffScreenPhase();
		void StartMoveOffScreenWaitPhase();
		void UpdateMoveOffScreenWaitPhase();
		void StartShootPhase();
		void UpdateShootPhase();
		void StartShootWaitPhase();
		void UpdateShootWaitPhase();
		void StartReturnCenterPhase();
		void UpdateReturnCenterPhase();

		GunAction_GrenadeLauncherPhase m_phase;

		Value_ptr<RelativeTimer> m_vlp_moveOffScreenPhaseTimer;

		Value_ptr<RelativeTimer> m_vlp_moveOffScreenWaitPhaseTimer;

		Value_ptr<RelativeTimer> m_vlp_shootPhaseTimer;

		Value_ptr<RelativeTimer> m_vlp_shootWaitPhaseTimer;

		Value_ptr<RelativeTimer> m_vlp_returnCenterPhaseTimer;
		Value_ptr<Transform> m_vlp_startTransform;

		Value_weak_ptr<Player> m_vwp_playerComponent;
		Value_weak_ptr<Gun> m_vwp_rightGunComponent;
		Value_weak_ptr<Gun> m_vwp_leftGunComponent;
		Value_weak_ptr<GameObject> m_vwp_drawObject;
	};

}

BUTI_REGIST_GAMECOMPONENT(GunAction_GrenadeLauncher, true);