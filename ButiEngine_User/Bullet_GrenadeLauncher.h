#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Bullet_GrenadeLauncher :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "Bullet_GrenadeLauncher";
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

		void SetVelocity(const Vector3& arg_velocity) { m_velocity = arg_velocity; }

		void Dead();
	private:
		Value_ptr<RelativeTimer> m_vlp_lifeTimer;
		Vector3 m_velocity;
	};

}

BUTI_REGIST_GAMECOMPONENT(Bullet_GrenadeLauncher, true);