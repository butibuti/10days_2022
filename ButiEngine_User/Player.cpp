#include "stdafx_u.h"
#include "Player.h"
#include "InputManager.h"
#include "Header/GameObjects/DefaultGameComponent/RigidBodyComponent.h"
#include "SeparateDrawObject.h"
#include "Gun.h"
#include "EquipGun.h"
#include "Bullet.h"
#include "BaseEnemy.h"
#include "LesserEnemy.h"
#include "BossEnemy.h"
#include "GunAction_BossSpin.h"
#include "EnemySpawner.h"
#include "PreAction.h"
#include "Camera.h"

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
	m_vwp_rigidBodyComponent = gameObject.lock()->GetGameComponent<RigidBodyComponent>();

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

	GetManager().lock()->GetGameObject("CameraMan").lock()->GetGameComponent<Camera>()->StopChasePlayer();

	auto rigidBodyComponent = gameObject.lock()->GetGameComponent<RigidBodyComponent>();
	rigidBodyComponent->GetRigidBody()->SetVelocity(Vector3Const::Zero);
	rigidBodyComponent->UnRegist();

	//現在出ている弾を停止
	auto bullets = GetManager().lock()->GetGameObjects(GameObjectTag("Bullet"));
	for (auto bullet : bullets)
	{
		bullet->GetGameComponent<Bullet>()->StartPause();
	}
	auto enemies = GetManager().lock()->GetGameObjects(GameObjectTag("Enemy"));
	for (auto enemy : enemies)
	{
		if (enemy->HasGameObjectTag("BaseEnemy"))
		{
			enemy->GetGameComponent<BaseEnemy>()->StartPause();
		}
		else if (enemy->HasGameObjectTag("LesserEnemy"))
		{
			enemy->GetGameComponent<LesserEnemy>()->StartPause();
		}
		else if (enemy->HasGameObjectTag("BossEnemy"))
		{
			enemy->GetGameComponent<BossEnemy>()->StartPause();
		}
	}
	auto enemySpawner = GetManager().lock()->GetGameObject("EnemySpawner").lock()->GetGameComponent<EnemySpawner>();
	enemySpawner->StartPause();
}

void ButiEngine::Player::FinishGunAction()
{
	m_canAcceptInput = true;
	m_isInvincible = false;
	m_canPickUpItem = true;

	GetManager().lock()->GetGameObject("CameraMan").lock()->GetGameComponent<Camera>()->StartChasePlayer();

	auto rigidBodyComponent = gameObject.lock()->GetGameComponent<RigidBodyComponent>();
	rigidBodyComponent->Regist();

	//弾のPauseを解除
	auto bullets = GetManager().lock()->GetGameObjects(GameObjectTag("Bullet"));
	for (auto bullet : bullets)
	{
		bullet->GetGameComponent<Bullet>()->FinishPause();
	}
	auto enemies = GetManager().lock()->GetGameObjects(GameObjectTag("Enemy"));
	for (auto enemy : enemies)
	{
		if (enemy->HasGameObjectTag("BaseEnemy"))
		{
			enemy->GetGameComponent<BaseEnemy>()->FinishPause();
		}
		else if (enemy->HasGameObjectTag("LesserEnemy"))
		{
			enemy->GetGameComponent<LesserEnemy>()->FinishPause();
		}
		else if (enemy->HasGameObjectTag("BossEnemy"))
		{
			enemy->GetGameComponent<BossEnemy>()->FinishPause();
		}
	}
	auto enemySpawner = GetManager().lock()->GetGameObject("EnemySpawner").lock()->GetGameComponent<EnemySpawner>();
	enemySpawner->FinishPause();
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

ButiEngine::Value_weak_ptr<ButiEngine::GameObject> ButiEngine::Player::ChangeGun(const std::string& arg_gunName)
{
	auto newGun = m_vwp_equipGunComponent.lock()->ChangeGun(arg_gunName);
	m_vwp_gunComponent = newGun.lock()->GetGameComponent<Gun>();
	return newGun;
}

void ButiEngine::Player::EquipAssaultRifle()
{
	StartGunAction();
	auto preActionComponent = gameObject.lock()->AddGameComponent<PreAction>(GunActionType::AssaultRifle);
}

void ButiEngine::Player::EquipGrenadeLauncher()
{
	StartGunAction();
	auto preActionComponent = gameObject.lock()->AddGameComponent<PreAction>(GunActionType::GrenadeLauncher);
}

void ButiEngine::Player::EquipShotgun()
{
	StartGunAction();
	auto preActionComponent = gameObject.lock()->AddGameComponent<PreAction>(GunActionType::Shotgun);
}

void ButiEngine::Player::EquipAllGun()
{
	//クリア演出用
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
	m_vwp_rigidBodyComponent.lock()->GetRigidBody()->SetVelocity(velocity * GameDevice::GetWorldSpeed());
}

void ButiEngine::Player::Rotate()
{
	auto lookTarget = m_vwp_lookAtComponent.lock()->GetLookTarget();
	auto drawObjectTransform = m_vwp_drawObject.lock()->transform;
	lookTarget->SetLocalPosition(drawObjectTransform->GetWorldPosition() + drawObjectTransform->GetFront() * 100.0f);

	Vector2 rightStick = InputManager::GetRightStick();
	if (rightStick != 0.0f)
	{
		lookTarget->SetLocalPosition(drawObjectTransform->GetWorldPosition());
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
		//EquipGrenadeLauncher();
		EquipShotgun();
	}
}

void ButiEngine::Player::Damage(const int32_t arg_power)
{
	if (m_isInvincible)
	{
		return;
	}
	if (m_hitPoint <= 0)
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
	m_vwp_lookAtComponent = m_vwp_drawObject.lock()->GetGameComponent<LookAtComponent>();
	auto drawObjectTransform = m_vwp_drawObject.lock()->transform;
	m_vwp_lookAtComponent.lock()->SetLookTarget(ObjectFactory::Create<Transform>(drawObjectTransform->GetWorldPosition()));
	m_vwp_lookAtComponent.lock()->GetLookTarget()->Translate(drawObjectTransform->GetFront() * 100.0f);
	m_vwp_lookAtComponent.lock()->SetSpeed(0.1f);
}
