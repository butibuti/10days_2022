#include "stdafx_u.h"
#include "Player.h"
#include "InputManager.h"
#include "Header/GameObjects/DefaultGameComponent/RigidBodyComponent.h"
#include "SeparateDrawObject.h"
#include "Gun.h"
#include "EquipGun.h"
#include "Bullet.h"
#include "GunAction_AssaultRifle.h"
#include "GunAction_GrenadeLauncher.h"

void ButiEngine::Player::OnUpdate()
{
	Control();
}

void ButiEngine::Player::OnSet()
{
	//Bullet_Enemyタグを持っていたらダメージを受ける
	gameObject.lock()->AddCollisionEnterReaction(
		[this](ButiBullet::ContactData& arg_other)
		{
			if (arg_other.vwp_gameObject.lock())
			{
				//タグ判定
				if (arg_other.vwp_gameObject.lock()->HasGameObjectTag("Bullet_Enemy") && !m_isInvincible)
				{
					auto bulletComponent = arg_other.vwp_gameObject.lock()->GetGameComponent<Bullet>();

					if (bulletComponent && !ExistInHitComponent(bulletComponent))
					{
						m_vec_vwp_hitComponent.push_back(bulletComponent);
						Damage(bulletComponent->GetPower());
					}
				}
			}
		}
	);
}

void ButiEngine::Player::Start()
{
	m_vwp_rigidBody = gameObject.lock()->GetGameComponent<RigidBodyComponent>();

	m_vwp_drawObject = gameObject.lock()->GetGameComponent<SeparateDrawObject>()->GetDrawObject().lock();

	m_vwp_equipGunComponent = gameObject.lock()->GetGameComponent<EquipGun>();
	m_vwp_gunComponent = m_vwp_equipGunComponent.lock()->GetGun().lock()->GetGameComponent<Gun>();

	m_hitPoint = m_maxHitPoint;
	m_vlp_invincibleTime = ObjectFactory::Create<RelativeTimer>(m_invincibleInterval);
	m_vlp_invincibleTime->Start();
	m_isInvincible = false;
	m_vec_vwp_hitComponent.clear();

	SetLookAtParameter();

	m_canAcceptInput = true;
	m_canPickUpItem = true;
}

void ButiEngine::Player::OnRemove()
{
}

void ButiEngine::Player::OnShowUI()
{
	GUI::BulletText(u8"最大体力");
	if (GUI::DragInt("##maxHitPoint", &m_maxHitPoint))
	{
		m_hitPoint = m_maxHitPoint;
	}
	GUI::Text(m_hitPoint);

	GUI::BulletText(u8"無敵時間");
	if (GUI::DragInt("##invincibleInterval", &m_invincibleInterval, 1.0f, 1.0f, 30.0f))
	{
		if (m_vlp_invincibleTime)
		{
			m_vlp_invincibleTime->ChangeCountFrame(m_invincibleInterval);
		}
	}
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Player::Clone()
{
	return ObjectFactory::Create<Player>();
}

void ButiEngine::Player::StartGunAction()
{
	m_canAcceptInput = false;
	m_isInvincible = true;
	m_canPickUpItem = false;
}

void ButiEngine::Player::FinishGunAction()
{
	m_canAcceptInput = true;
	m_isInvincible = false;
	m_canPickUpItem = true;
}

void ButiEngine::Player::PowerUp(const std::string& arg_gunName)
{
	auto newGun = m_vwp_equipGunComponent.lock()->ChangeGun(arg_gunName);
	m_vwp_gunComponent = newGun.lock()->GetGameComponent<Gun>();
}

void ButiEngine::Player::Dead()
{
	gameObject.lock()->GetGameComponent<SeparateDrawObject>()->Dead();
	m_vwp_equipGunComponent.lock()->Dead();
	gameObject.lock()->SetIsRemove(true);
}

ButiEngine::Value_weak_ptr<ButiEngine::Gun> ButiEngine::Player::ChangeGun(const std::string& arg_gunName)
{
	auto newGun = m_vwp_equipGunComponent.lock()->ChangeGun(arg_gunName);
	m_vwp_gunComponent = newGun.lock()->GetGameComponent<Gun>();
	return m_vwp_gunComponent;
}

void ButiEngine::Player::EquipAssaultRifle()
{
	gameObject.lock()->AddGameComponent<GunAction_AssaultRifle>();
}

void ButiEngine::Player::EquipGrenadeLauncher()
{
	gameObject.lock()->AddGameComponent<GunAction_GrenadeLauncher>();
}

void ButiEngine::Player::Control()
{
	if (!m_canAcceptInput)
	{ 
		m_isInvincible = true;
		return;
	}

	//直前フレームにダメージがあったかを判定
	CheckHasDamageInPreviousFrame();

	Move();
	Rotate();
	Shoot();

	if (m_vlp_invincibleTime->Update())
	{
		m_isInvincible = false;
	}
}

void ButiEngine::Player::Move()
{
	Vector2 leftStick = InputManager::GetLeftStick();
	float speed = 5.0f;
	Vector3 velocity = Vector3(leftStick.x, 0.0f, leftStick.y).Normalize() * speed;
	m_vwp_rigidBody.lock()->GetRigidBody()->SetVelocity(velocity * GameDevice::GetWorldSpeed());
}

void ButiEngine::Player::Rotate()
{
	auto lookTarget = m_vwp_lookAt.lock()->GetLookTarget();
	auto drawObjectTransform = m_vwp_drawObject.lock()->transform;
	lookTarget->SetLocalPosition(drawObjectTransform->GetWorldPosition() + drawObjectTransform->GetFront() * 100.0f);

	Vector2 rightStick = InputManager::GetRightStick();
	if (rightStick != 0.0f)
	{
		lookTarget->Translate(Vector3(rightStick.x, 0.0f, rightStick.y).Normalize() * 100.0f);
	}
}

void ButiEngine::Player::Shoot()
{
	if (InputManager::IsTriggerShootKey())
	{
		m_vwp_gunComponent.lock()->ShootStart();
	}
	else if (InputManager::IsReleaseShootKey())
	{
		m_vwp_gunComponent.lock()->ShootStop();
	}

	if (InputManager::IsTriggerCancelKey())
	{
		//EquipAssaultRifle();
		EquipGrenadeLauncher();
	}
}

void ButiEngine::Player::Damage(const int32_t arg_power)
{
	if (m_isInvincible)
	{
		return;
	}

	m_hitPoint -= arg_power;

	if (m_hitPoint <= 0)
	{
		//Dead();
		m_hitPoint = 0;
	}
}

bool ButiEngine::Player::ExistInHitComponent(Value_weak_ptr<GameComponent> arg_vwp_hitComponent)
{
	for (auto hitComponent : m_vec_vwp_hitComponent)
	{
		if (hitComponent == arg_vwp_hitComponent)
		{
			return true;
		}
	}

	return false;
}

void ButiEngine::Player::CheckHasDamageInPreviousFrame()
{
	if (m_vec_vwp_hitComponent.empty())
	{
		return;
	}

	m_isInvincible = true;
	m_vlp_invincibleTime->Reset();
	m_vec_vwp_hitComponent.clear();
}

void ButiEngine::Player::SetLookAtParameter()
{
	m_vwp_lookAt = m_vwp_drawObject.lock()->GetGameComponent<LookAtComponent>();
	auto drawObjectTransform = m_vwp_drawObject.lock()->transform;
	m_vwp_lookAt.lock()->SetLookTarget(ObjectFactory::Create<Transform>(drawObjectTransform->GetWorldPosition()));
	m_vwp_lookAt.lock()->GetLookTarget()->Translate(drawObjectTransform->GetFront() * 100.0f);
	m_vwp_lookAt.lock()->SetSpeed(0.3f);
}
