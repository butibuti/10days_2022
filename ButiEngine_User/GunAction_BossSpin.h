#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class BossEnemy;
	class Gun;

	enum class GunAction_BossSpinPhase
	{
		BeforeShoot,
		Shoot,
		AfterShoot
	};

	class GunAction_BossSpin :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "GunAction_BossSpin";
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
		void StartBeforeShootPhase();
		void UpdateBeforeShootPhase();
		void StartShootPhase();
		void UpdateShootPhase();
		void StartAfterShootPhase();
		void UpdateAfterShootPhase();
		void Rotate();

		GunAction_BossSpinPhase m_phase;

		Value_ptr<RelativeTimer> m_vlp_waitTimer;
		std::int32_t m_waitTime = 20;
		Value_ptr<RelativeTimer> m_vlp_actionTimer;
		std::int32_t m_actionTime = 300;
		Value_ptr<Transform> m_vwp_startTransform;

		Value_weak_ptr<BossEnemy> m_vwp_bossEnemyComponent;
		Value_weak_ptr<Gun> m_vwp_rightGunComponent;
		Value_weak_ptr<Gun> m_vwp_leftGunComponent;
		Value_weak_ptr<GameObject> m_vwp_drawObject;
	};

}

BUTI_REGIST_GAMECOMPONENT(GunAction_BossSpin, true);