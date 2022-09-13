#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Player;
	class Gun;

	enum class GunAction_ShotgunPhase
	{
		Move,
		MoveWait,
		Shoot,
		Return,
	};

	class GunAction_Shotgun :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "GunAction_Shotgun";
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
		void StartMovePhase();
		void UpdateMovePhase();
		void StartMoveWaitPhase();
		void UpdateMoveWaitPhase();
		void StartShootPhase();
		void UpdateShootPhase();
		void AddShootAnimation();
		void StartReturnPhase();
		void UpdateReturnPhase();

		GunAction_ShotgunPhase m_phase;

		Value_ptr<RelativeTimer> m_vlp_movePhaseTimer;

		Value_ptr<RelativeTimer> m_vlp_moveWaitPhaseTimer;

		Value_ptr<RelativeTimer> m_vlp_shootPhaseTimer;
		Value_ptr<RelativeTimer> m_vlp_moveShootTimer;
		Value_ptr<Transform> m_vlp_moveShootCenterTransform;
		std::int32_t m_maxShootCount;
		std::int32_t m_shootCount;
		float m_radius;

		Value_ptr<RelativeTimer> m_vlp_returnPhaseTimer;

		Value_weak_ptr<Player> m_vwp_playerComponent;
		Value_weak_ptr<Gun> m_vwp_gunComponent;
		Value_weak_ptr<GameObject> m_vwp_drawObject;
	};

}

BUTI_REGIST_GAMECOMPONENT(GunAction_Shotgun, true);