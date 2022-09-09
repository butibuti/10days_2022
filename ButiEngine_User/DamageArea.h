#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {
	class DamageArea :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "DamageArea";
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
			archive(m_life);
			archive(m_power);
			archive(m_giveDamageIntervalFrame);
		}

		void Dead();
		bool CanGiveDamage() { return m_canGiveDamage; }
	private:
		Value_ptr<RelativeTimer> m_vlp_lifeTimer;
		std::int32_t m_life = 1;

		//ダメージ関連
		float m_power = 10.0f;
		std::int32_t m_giveDamageIntervalFrame = 0;
		Value_ptr<RelativeTimer> m_vlp_giveDamageInterval;
		bool m_canGiveDamage;
	};

}

BUTI_REGIST_GAMECOMPONENT(DamageArea, true);