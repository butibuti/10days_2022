#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class BossEnemy;
	class Gun;
	class Player;

	enum class GunAction_BossSnakePhase
	{
		BeforeShoot,
		Shoot,
		AfterShoot
	};

	class GunAction_BossSnake :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "GunAction_BossSnake";
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

		void StartPause();
		void FinishPause();
	private:
		void StartBeforeShootPhase();
		void UpdateBeforeShootPhase();
		void StartShootPhase();
		void UpdateShootPhase();
		void StartAfterShootPhase();
		void UpdateAfterShootPhase();
		void Rotate();

		GunAction_BossSnakePhase m_phase;

		Value_ptr<RelativeTimer> m_vlp_waitTimer;
		std::int32_t m_lockOnTime;
		std::int32_t m_restTime;
		Value_ptr<RelativeTimer> m_vlp_actionTimer;
		std::int32_t m_actionTime;
		Value_ptr<Transform> m_vwp_startTransform;
		std::int32_t m_turnCount;
		std::int32_t m_turnCountMax;
		std::int32_t m_spreadDegree;

		Value_weak_ptr<BossEnemy> m_vwp_bossEnemyComponent;
		Value_weak_ptr<Gun> m_vwp_gunComponent;
		Value_weak_ptr<GameObject> m_vwp_drawObject;
		Value_weak_ptr<GameObject> m_vwp_player;
		Value_weak_ptr<LookAtComponent> m_vwp_lookAt;
	};

}

BUTI_REGIST_GAMECOMPONENT(GunAction_BossSnake, true);