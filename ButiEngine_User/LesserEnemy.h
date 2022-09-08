#pragma once
#include"Header/GameComponentHeader.h"
#include "BaseEnemy.h"
namespace ButiEngine {

	class RigidBodyComponent;
	class Gun;
	class EquipGun;

	class LesserEnemy :public BaseEnemy
	{
	public:

		std::string GetGameComponentName()const override {
			return "LesserEnemy";
		}
		void OnUpdate()override;
		void OnShowUI()override;
		Value_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
			archive(m_speed);
			archive(m_directionDicisionInterval);
			archive(m_minimumDistance);
			archive(m_maxHitPoint);
			archive(m_invincivleInterval);
			archive(m_attackInterval);
		}

		void Dead()override;
	private:
		void Control()override;
		void Move()override;
		void Rotate()override;
		void DecideDirection()override;
		void Attack()override;
		void Damage(const int32_t arg_power)override;
	};

}

BUTI_REGIST_GAMECOMPONENT(LesserEnemy, true);