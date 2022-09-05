#pragma once
#include"Header/GameComponentHeader.h"

namespace ButiEngine {

	class WorldSpeedManager :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "WorldSpeedManager";
		}
		void OnUpdate()override;
		void OnSet()override;
		void OnShowUI()override;
		void Start()override;
		Value_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}

		static void SetSpeed(const float arg_speed) { GameDevice::WorldSpeed = arg_speed; }
		static void SetSpeed(const float arg_speed, const std::uint16_t arg_returnSpeedFrame)
		{
			if (g_isChangeSpeed) { return; }

			SetSpeed(arg_speed);
			g_vlp_timer->ChangeCountFrame(arg_returnSpeedFrame);
			g_vlp_timer->Start();
			g_isChangeSpeed = true;
		}
	private:
		void OnChangeSpeed();

		static Value_ptr<Timer> g_vlp_timer;
		static bool g_isChangeSpeed;
	};

}

BUTI_REGIST_GAMECOMPONENT(WorldSpeedManager, true);