#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class BossEnemy;
	class Gun;

	enum class GunAction_BossLauncherPhase
	{
		LockOn,
		Shoot,
		Rest,
		AfterShoot
	};

	class GunAction_BossLauncher :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "GunAction_BossLauncher";
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
		void StartLockOnPhase();
		void UpdateLockOnPhase();
		void StartShootPhase();
		void UpdateShootPhase();
		void StartRestPhase();
		void UpdateRestPhase();
		void StartAfterShootPhase();
		void UpdateAfterShootPhase();
		void Rotate();

		GunAction_BossLauncherPhase m_phase;

		Value_ptr<RelativeTimer> m_vlp_waitTimer;
		std::int32_t m_lockOnTime;
		std::int32_t m_restTime;
		std::int32_t m_afterTime;
		Value_ptr<RelativeTimer> m_vlp_actionTimer;
		std::int32_t m_actionTime;
		Value_ptr<Transform> m_vwp_startTransform;
		std::int32_t m_shootCount;
		std::int32_t m_shootCountMax;

		Value_weak_ptr<BossEnemy> m_vwp_bossEnemyComponent;
		Value_weak_ptr<Gun> m_vwp_gunComponent;
		Value_weak_ptr<GameObject> m_vwp_drawObject;
	};

}

BUTI_REGIST_GAMECOMPONENT(GunAction_BossLauncher, true);