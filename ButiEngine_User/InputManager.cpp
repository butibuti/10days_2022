#include "stdafx_u.h"
#include "InputManager.h"
#include<Xinput.h>

const float ButiEngine::InputManager::g_DEADZONE = 0.25f;
bool ButiEngine::InputManager::g_isNotPushKey;
ButiEngine::Vector2 ButiEngine::InputManager::g_previousRightStick;
ButiEngine::Vector2 ButiEngine::InputManager::g_currentRightStick;
ButiEngine::Vector2 ButiEngine::InputManager::g_previousLeftStick;
ButiEngine::Vector2 ButiEngine::InputManager::g_currentLeftStick;

void ButiEngine::InputManager::OnUpdate()
{
	g_previousRightStick = g_currentRightStick;
	g_previousLeftStick = g_currentLeftStick;

	g_currentRightStick = GameDevice::GetInput()->GetRightStick();
	g_currentLeftStick = GameDevice::GetInput()->GetLeftStick();
}

void ButiEngine::InputManager::OnSet()
{
}

void ButiEngine::InputManager::Start()
{
	g_previousLeftStick = Vector2();
	g_currentLeftStick = Vector2();
	g_previousRightStick = Vector2();
	g_currentRightStick = Vector2();
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::InputManager::Clone()
{
	return ObjectFactory::Create<InputManager>();
}

bool ButiEngine::InputManager::IsPushRightKey()
{
	return (GameDevice::GetInput()->CheckKey(Keys::D) ||
		GameDevice::GetInput()->GetPadButton(PadButtons::XBOX_RIGHT) ||
		g_currentRightStick.x >= g_DEADZONE ||
		g_currentLeftStick.x >= g_DEADZONE);
}

bool ButiEngine::InputManager::IsPushLeftKey()
{
	return (GameDevice::GetInput()->CheckKey(Keys::A) ||
		GameDevice::GetInput()->GetPadButton(PadButtons::XBOX_LEFT) ||
		g_currentRightStick.x <= -g_DEADZONE ||
		g_currentLeftStick.x <= -g_DEADZONE);
}

bool ButiEngine::InputManager::IsPushUpKey()
{
	return (GameDevice::GetInput()->CheckKey(Keys::W) ||
		GameDevice::GetInput()->GetPadButton(PadButtons::XBOX_UP) ||
		g_currentRightStick.y >= g_DEADZONE ||
		g_currentLeftStick.y >= g_DEADZONE);
}

bool ButiEngine::InputManager::IsPushDownKey()
{
	return (GameDevice::GetInput()->CheckKey(Keys::S) ||
		GameDevice::GetInput()->GetPadButton(PadButtons::XBOX_DOWN) ||
		g_currentRightStick.y <= -g_DEADZONE ||
		g_currentLeftStick.y <= -g_DEADZONE);
}

bool ButiEngine::InputManager::IsTriggerRightKey()
{
	return (GameDevice::GetInput()->TriggerKey(Keys::D) ||
		GameDevice::GetInput()->GetPadButtonTrigger(PadButtons::XBOX_RIGHT) ||
		(g_currentRightStick.x >= g_DEADZONE && g_previousRightStick.x < g_DEADZONE) ||
		(g_currentLeftStick.x >= g_DEADZONE && g_previousLeftStick.x < g_DEADZONE));
}

bool ButiEngine::InputManager::IsTriggerLeftKey()
{
	return (GameDevice::GetInput()->TriggerKey(Keys::A) ||
		GameDevice::GetInput()->GetPadButtonTrigger(PadButtons::XBOX_LEFT) ||
		(g_currentRightStick.x <= -g_DEADZONE && g_previousRightStick.x > -g_DEADZONE) ||
		(g_currentLeftStick.x <= -g_DEADZONE && g_previousLeftStick.x > -g_DEADZONE));
}

bool ButiEngine::InputManager::IsTriggerUpKey()
{
	return (GameDevice::GetInput()->TriggerKey(Keys::W) ||
		GameDevice::GetInput()->GetPadButtonTrigger(PadButtons::XBOX_UP) ||
		(g_currentRightStick.y >= g_DEADZONE && g_previousRightStick.y < g_DEADZONE) ||
		(g_currentLeftStick.y >= g_DEADZONE && g_previousLeftStick.y < g_DEADZONE));
}

bool ButiEngine::InputManager::IsTriggerDownKey()
{
	return (GameDevice::GetInput()->TriggerKey(Keys::S) ||
		GameDevice::GetInput()->GetPadButtonTrigger(PadButtons::XBOX_DOWN) ||
		(g_currentRightStick.y <= -g_DEADZONE && g_previousRightStick.y > -g_DEADZONE) ||
		(g_currentLeftStick.y <= -g_DEADZONE && g_previousLeftStick.y > -g_DEADZONE));
}

bool ButiEngine::InputManager::IsTriggerDecideKey()
{
	return (GameDevice::GetInput()->TriggerKey(Keys::Space) ||
		GameDevice::GetInput()->GetPadButtonTrigger(PadButtons::XBOX_A));
}

bool ButiEngine::InputManager::IsTriggerCancelKey()
{
	return (GameDevice::GetInput()->TriggerKey(Keys::Esc) ||
		GameDevice::GetInput()->GetPadButtonTrigger(PadButtons::XBOX_B));
}

bool ButiEngine::InputManager::IsPushAnyGameKey()
{
	return (IsPushRightKey() ||
		IsPushLeftKey() ||
		IsPushUpKey() ||
		IsPushDownKey());
}

ButiEngine::Vector2 ButiEngine::InputManager::GetMousePosition()
{
	return GameDevice::GetInput()->GetMousePos();
}

void ButiEngine::InputManager::SetRightVibrationPower(const float arg_power)
{
	GameDevice::GetInput()->Vibration_R(arg_power);
}

void ButiEngine::InputManager::SetLeftVibrationPower(const float arg_power)
{
	GameDevice::GetInput()->Vibration_L(arg_power);
}

bool ButiEngine::InputManager::IsPushShootKey()
{
	return (GameDevice::GetInput()->GetMouseTrigger(MouseButtons::LeftClick) ||
		GameDevice::GetInput()->GetPadButtonTrigger(PadButtons::XBOX_A));
}

bool ButiEngine::InputManager::IsTriggerPauseKey()
{
	return (GameDevice::GetInput()->TriggerKey(Keys::Esc) ||
		GameDevice::GetInput()->GetPadButtonTrigger(PadButtons::XBOX_START));
}

ButiEngine::Vector2 ButiEngine::InputManager::GetRightStick()
{
	Vector2 output = g_currentRightStick;

	if (abs(output.x) < g_DEADZONE) { output.x = 0.0f; }
	if (abs(output.y) < g_DEADZONE) { output.y = 0.0f; }
	return output;
}

ButiEngine::Vector2 ButiEngine::InputManager::GetLeftStick()
{
	Vector2 output = g_currentLeftStick;

	if (abs(output.x) < g_DEADZONE) { output.x = 0.0f; }
	if (abs(output.y) < g_DEADZONE) { output.y = 0.0f; }
	return output;
}

float ButiEngine::InputManager::GetRightTrigger()
{
	float output = GameDevice::GetInput()->GetRightTrigger();

	if (output < g_DEADZONE) { output = 0.0f; }

	return output;
}

float ButiEngine::InputManager::GetLeftTrigger()
{
	float output = GameDevice::GetInput()->GetLeftTrigger();

	if (output < g_DEADZONE) { output = 0.0f; }

	return output;
}
