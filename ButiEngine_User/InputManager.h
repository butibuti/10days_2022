#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class InputManager :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "InputManager";
		}
		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		Value_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}

		//ゲーム全体
		static bool IsPushRightKey();
		static bool IsPushLeftKey();
		static bool IsPushUpKey();
		static bool IsPushDownKey();
		static bool IsTriggerRightKey();
		static bool IsTriggerLeftKey();
		static bool IsTriggerUpKey();
		static bool IsTriggerDownKey();
		static bool IsTriggerDecideKey();
		static bool IsTriggerCancelKey();
		static bool IsPushAnyGameKey();
		static Vector2 GetMousePosition();

		static void SetRightVibrationPower(const float arg_power);
		static void SetLeftVibrationPower(const float arg_power);

		//ゲームプレイシーンのみ
		static bool IsPushShootKey();
		static bool IsReleaseShootKey();
		static bool IsTriggerPauseKey();

		static Vector2 GetRightStick();
		static Vector2 GetLeftStick();
		static float GetRightTrigger();
		static float GetLeftTrigger();
		static bool IsNotPushKey() { return g_isNotPushKey; }
	private:
		static const float g_DEADZONE;
		static bool g_isNotPushKey;
		static Vector2 g_previousRightStick;
		static Vector2 g_currentRightStick;
		static Vector2 g_previousLeftStick;
		static Vector2 g_currentLeftStick;
	};

}

BUTI_REGIST_GAMECOMPONENT(InputManager, true);