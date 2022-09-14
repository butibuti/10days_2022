#pragma once
#include"Header/GameComponentHeader.h"
#include "BaseEnemy.h"
namespace ButiEngine {

	class RigidBodyComponent;
	class Gun;
	class EquipGun;

	class BossEnemy :public BaseEnemy
	{
	public:

		std::string GetGameComponentName()const override {
			return "BossEnemy";
		}
		void OnUpdate()override;
		void Start()override;
		void OnShowUI()override;
		Value_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
			archive(m_defaultSpeed);
			archive(m_defaultDirectionDicisionInterval);
			archive(m_minimumDistance);
			archive(m_maxHitPoint);
			archive(m_invincibleInterval);
			archive(m_attackInterval);
		}

		void Dead()override;

		void StartGunAction();
		void FinishGunAction();

		void StartPause()override;
		void FinishPause()override;

		const bool IsPassedOut()const { return m_isPassedOut; }
	private:
		void Control()override;
		void Move()override;
		void Rotate()override;
		void DecideDirection()override;
		void Attack()override;
		void Damage(const int32_t arg_power)override;
		void EmitItem()override;

		void DeleteEnemySideObject();
		void CorrectDamageOnPhase();

		bool m_canMove;
		//気絶している
		bool m_isPassedOut;
		//行動フェーズ用
		std::int32_t m_phaseCount;
		std::int32_t m_phaseCountMax = 4;
	};

}

BUTI_REGIST_GAMECOMPONENT(BossEnemy, true);