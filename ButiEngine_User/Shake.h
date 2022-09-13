#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {
	class Shake :public GameComponent
	{
	public:
		Shake(){}
		Shake(const std::int32_t arg_shakeFrame, const Vector3& arg_amplitude, const std::int32_t arg_shakeIntervalFrame)
			: m_shakeFrame(arg_shakeFrame), m_amplitude(arg_amplitude), m_shakeIntervalFrame(arg_shakeIntervalFrame + 1)
		{
		}

		std::string GetGameComponentName()const override {
			return "Shake";
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

		void Move();
	private:
		Value_ptr<RelativeTimer> m_vlp_shakeTimer;
		Value_ptr<RelativeTimer> m_vlp_shakeIntervalTimer;

		std::int32_t m_shakeFrame;
		std::int32_t m_shakeIntervalFrame;

		Vector3 m_startPos;
		Vector3 m_amplitude;
	};

}

BUTI_REGIST_GAMECOMPONENT(Shake, true);