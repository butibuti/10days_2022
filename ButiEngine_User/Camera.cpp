#include "stdafx_u.h"
#include "Camera.h"
#include "Header/GameObjects/DefaultGameComponent/PositionAnimationComponent.h"
#include "Header/GameObjects/DefaultGameComponent/RotationAnimationComponent.h"
#include "Shake.h"

void ButiEngine::Camera::OnUpdate()
{
}

void ButiEngine::Camera::OnSet()
{
}

void ButiEngine::Camera::Start()
{
	m_vwp_parent = GetManager().lock()->GetGameObject("CameraParent");

	m_vlp_defaultTransform = gameObject.lock()->transform->Clone();
	m_vlp_parentDefaultTransform = m_vwp_parent.lock()->transform->Clone();
}

void ButiEngine::Camera::OnRemove()
{
}

void ButiEngine::Camera::OnShowUI()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Camera::Clone()
{
	return ObjectFactory::Create<Camera>();
}

void ButiEngine::Camera::StartChasePlayer()
{
	m_vwp_parent.lock()->GetGameComponent<ChaseComponent>()->SetIsActive(true);
}

void ButiEngine::Camera::StopChasePlayer()
{
	m_vwp_parent.lock()->GetGameComponent<ChaseComponent>()->SetIsActive(false);
}

void ButiEngine::Camera::Approach(const std::int32_t arg_frame)
{
	Vector3 targetPos = gameObject.lock()->transform->GetLocalPosition();
	targetPos += gameObject.lock()->transform->GetFront() * 10.0f;

	auto cameraAnim = gameObject.lock()->AddGameComponent<PositionAnimation>();
	cameraAnim->SetInitPosition(gameObject.lock()->transform->GetLocalPosition());
	cameraAnim->SetTargetPosition(targetPos);
	cameraAnim->SetSpeed(1.0f / arg_frame);
	cameraAnim->SetEaseType(Easing::EasingType::EaseOutCirc);

	auto targetRotation = m_vwp_parent.lock()->transform->Clone();
	targetRotation->RollLocalRotationX_Degrees(-50.0f);

	auto parentAnim = m_vwp_parent.lock()->AddGameComponent<RotationAnimation>();
	parentAnim->SetInitRotate(m_vwp_parent.lock()->transform->GetLocalRotation());
	parentAnim->SetTargetRotate(targetRotation->GetLocalRotation());
	parentAnim->SetSpeed(1.0f / arg_frame);
	parentAnim->SetEaseType(Easing::EasingType::EaseOutCirc);
}

void ButiEngine::Camera::ReturnDefault(const std::int32_t arg_frame)
{
	auto cameraAnim = gameObject.lock()->AddGameComponent<PositionAnimation>();
	cameraAnim->SetInitPosition(gameObject.lock()->transform->GetLocalPosition());
	cameraAnim->SetTargetPosition(m_vlp_defaultTransform->GetLocalPosition());
	cameraAnim->SetSpeed(1.0f / arg_frame);
	cameraAnim->SetEaseType(Easing::EasingType::EaseOutCirc);

	auto parentAnim = m_vwp_parent.lock()->AddGameComponent<RotationAnimation>();
	parentAnim->SetInitRotate(m_vwp_parent.lock()->transform->GetLocalRotation());
	parentAnim->SetTargetRotate(m_vlp_parentDefaultTransform->GetLocalRotation());
	parentAnim->SetSpeed(1.0f / arg_frame);
	parentAnim->SetEaseType(Easing::EasingType::EaseOutCirc);
}

void ButiEngine::Camera::StartShake(const std::int32_t arg_shakeFrame, const Vector3& arg_amplitude, const std::int32_t arg_shakeIntervalFrame)
{
	auto shakeComponent = gameObject.lock()->GetGameComponent<Shake>();
	if (shakeComponent)
	{
		shakeComponent->Dead();
	}

	gameObject.lock()->AddGameComponent<Shake>(arg_shakeFrame, arg_amplitude, arg_shakeIntervalFrame);
}
