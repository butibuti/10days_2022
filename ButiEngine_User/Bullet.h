#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

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
			archive(m_penetratingCount);
		}

		const std::int32_t GetPower()const { return static_cast<std::int32_t>(m_power); }
		const bool IsHitInCurrentFrame()const { return m_isHitInCurrentFrame; }

		void SetOwner(Value_weak_ptr<GameObject> arg_owner) { m_vwp_owner = arg_owner; }
		void SetPower(const float arg_power) { m_power = arg_power; }
		void SetRange(const float arg_range) { m_range = arg_range; }
		void SetVelocity(const Vector3& arg_velocity) { m_velocity = arg_velocity; }

		void Dead();
	private:
		void AddHitObject(Value_weak_ptr<GameObject> arg_vwp_hitObject);

		Value_weak_ptr<GameObject> m_vwp_owner;

		float m_power;
		float m_range;
		Vector3 m_velocity;
		
		Vector3 m_startPos;

		bool m_isHitInCurrentFrame;

		std::int32_t m_penetratingCount;
		std::vector<Value_weak_ptr<GameObject>> m_vec_hitObjects;
	};

}

BUTI_REGIST_GAMECOMPONENT(Bullet, true);