#pragma once
#include"Header/GameComponentHeader.h"
#include "BaseEnemy.h"
namespace ButiEngine {

	class RigidBodyComponent;
	class Gun;
	class EquipGun;

	class TutorialEnemy :public BaseEnemy
	{
	public:

		std::string GetGameComponentName()const override {
			return "TutorialEnemy";
		}
		void OnUpdate()override;
		void OnSet()override;
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
	private:
		void Control()override;
		void Attack()override;
		void Damage(const int32_t arg_power)override;
		void EmitItem()override;
	};

}

BUTI_REGIST_GAMECOMPONENT(TutorialEnemy, true);