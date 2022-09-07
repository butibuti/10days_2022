#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class RigidBodyComponent;

	class Bullet :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "Bullet";
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

		void SetPower(const float arg_power) { m_power = arg_power; }
		void SetRange(const float arg_range) { m_range = arg_range; }
		void SetVelocity(const Vector3& arg_velocity) { m_velocity = arg_velocity; }

		void Dead();
	private:
		Value_weak_ptr<RigidBodyComponent> m_vwp_rigidBody;

		float m_power;
		float m_range;
		Vector3 m_velocity;
		
		Vector3 m_startPos;
	};

}

BUTI_REGIST_GAMECOMPONENT(Bullet, true);