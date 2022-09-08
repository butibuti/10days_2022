#include "stdafx_u.h"
#include "BaseEnemy.h"
#include "Header/GameObjects/DefaultGameComponent/RigidBodyComponent.h"
#include "SeparateDrawObject.h"
#include "Gun.h"
#include "EquipGun.h"
#include "Player.h"
#include "Bullet.h"

void ButiEngine::BaseEnemy::OnUpdate()
{
	Control();
}

void ButiEngine::BaseEnemy::OnSet()
{
	//Bullet_Playerƒ^ƒO‚ðŽ‚Á‚Ä‚¢‚½‚çƒ_ƒ[ƒW‚ðŽó‚¯‚é
	gameObject.lock()->AddCollisionEnterReaction(
		[this](ButiBullet::ContactData& arg_other)
		{
			if (arg_other.vwp_gameObject.lock())
			{
				//ƒ^ƒO”»’è
				if (arg_other.vwp_gameObject.lock()->HasGameObjectTag("Bullet_Player") && !m_isInvincivle)
				{
					Damage(arg_other.vwp_gameObject.lock()->GetGameComponent<Bullet>().Clone()->GetPower());
				}
			}
		}
	);
}

void ButiEngine::BaseEnemy::Start()
{
	m_vwp_rigidBody = gameObject.lock()->GetGameComponent<RigidBodyComponent>();
	m_vwp_drawObject = gameObject.lock()->GetGameComponent<SeparateDrawObject>()->GetDrawObject().lock();
	m_vwp_equipGunComponent = gameObject.lock()->GetGameComponent<EquipGun>();
	m_vwp_gunComponent = m_vwp_equipGunComponent.lock()->GetGun().lock()->GetGameComponent<Gun>();
	m_vwp_player = GetManager().lock()->GetGameObject("Player");
	m_vlp_directionDicisionTime = ObjectFactory::Create<RelativeTimer>(m_directionDicisionInterval);
	m_vlp_directionDicisionTime->Start();
	m_hitPoint = m_maxHitPoint;
	m_vlp_invincivleTime = ObjectFactory::Create<RelativeTimer>(m_invincivleInterval);
	m_vlp_invincivleTime->Start();
	m_isInvincivle = false;
	m_vlp_attackTime = ObjectFactory::Create<RelativeTimer>(m_attackInterval);
	m_vlp_attackTime->Start();
	m_direction = Vector3();
	SetLookAtParameter();
	DecideDirection();
}

void ButiEngine::BaseEnemy::OnRemove()
{
}

void ButiEngine::BaseEnemy::OnShowUI()
{
	GUI::BulletText(u8"ˆÚ“®‘¬“x");
	GUI::DragFloat("##speed", &m_speed, 1.0f, 0.0f, 10.0f);
	
	GUI::BulletText(u8"ˆÚ“®•ûŒüŒˆ’èŠÔŠu");
	if (GUI::DragInt("##moveRate", &m_directionDicisionInterval, 1.0f, 1.0f, 60.0f))
	{
		if (m_vlp_directionDicisionTime)
		{
			m_vlp_directionDicisionTime->ChangeCountFrame(m_directionDicisionInterval);
		}
	}
	
	GUI::BulletText(u8"‹ßÚ‹——£–Ú•W");
	GUI::DragFloat("##minimumDistance", &m_minimumDistance, 1.0f, 0.0f, 100.0f);
	
	GUI::BulletText(u8"Å‘å‘Ì—Í");
	if (GUI::DragInt("##maxHitPoint", &m_maxHitPoint))
	{
		m_hitPoint = m_maxHitPoint;
	}
	GUI::Text(m_hitPoint);
	
	GUI::BulletText(u8"–³“GŽžŠÔ");
	if (GUI::DragInt("##invincivleInterval", &m_invincivleInterval, 1.0f, 1.0f, 30.0f))
	{
		if (m_vlp_invincivleTime)
		{
			m_vlp_invincivleTime->ChangeCountFrame(m_invincivleInterval);
		}
	}
	
	GUI::BulletText(u8"UŒ‚ŠÔŠu");
	if (GUI::DragInt("##attackInterval", &m_attackInterval, 1.0f, 1.0f, 120.0f))
	{
		if (m_vlp_attackTime)
		{
			m_vlp_attackTime->ChangeCountFrame(m_attackInterval);
		}
	}
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::BaseEnemy::Clone()
{
	auto output = ObjectFactory::Create<BaseEnemy>();
	output->m_speed = m_speed;
	output->m_directionDicisionInterval = m_directionDicisionInterval;
	output->m_minimumDistance = m_minimumDistance;
	output->m_hitPoint = m_hitPoint;
	output->m_invincivleInterval = m_invincivleInterval;
	output->m_attackInterval = m_attackInterval;
	return output;
}

void ButiEngine::BaseEnemy::Dead()
{
	gameObject.lock()->GetGameComponent<SeparateDrawObject>()->Dead();
	m_vwp_equipGunComponent.lock()->Dead();
	gameObject.lock()->SetIsRemove(true);
}

void ButiEngine::BaseEnemy::Control()
{
	Move();
	Rotate();
	Attack();

	if (m_vlp_invincivleTime->Update())
	{
		m_isInvincivle = false;
	}
}

void ButiEngine::BaseEnemy::Move()
{
	//Player‚É‹ß‚Ã‚«‚½‚¢
	if (m_vlp_directionDicisionTime->Update())
	{
		DecideDirection();
	}
	m_vwp_rigidBody.lock()->GetRigidBody()->SetVelocity(m_direction * m_speed * GameDevice::WorldSpeed);
}

void ButiEngine::BaseEnemy::Rotate()
{
	auto lookTarget = m_vwp_lookAt.lock()->GetLookTarget();
	auto drawObjectTransform = m_vwp_drawObject.lock()->transform;
	lookTarget->SetLocalPosition(drawObjectTransform->GetLocalPosition() + drawObjectTransform->GetFront() * 100.0f);

	Vector3 playerPosition = m_vwp_player.lock()->GetGameComponent<RigidBodyComponent>()->GetRigidBody()->GetPosition();
	Vector3 position = m_vwp_rigidBody.lock()->GetRigidBody()->GetPosition();
	lookTarget->Translate(Vector3(playerPosition.x - position.x, 0.0f, playerPosition.z - position.z).Normalize() * 100.0f);
}

void ButiEngine::BaseEnemy::DecideDirection()
{
	m_vlp_directionDicisionTime->Reset();

	//‹——£ŒvŽZ
	Value_weak_ptr<RigidBodyComponent> playerRigidBody = m_vwp_player.lock()->GetGameComponent<RigidBodyComponent>();
	Vector3 playerPosition = playerRigidBody.lock()->GetRigidBody()->GetPosition();
	Vector3 position = m_vwp_rigidBody.lock()->GetRigidBody()->GetPosition();
	float distance = playerPosition.Distance(position);

	if (distance <= m_minimumDistance)
	{
		float sin, cos;
		MathHelper::SinCos(sin, cos, MathHelper::ToRadian(ButiRandom::GetInt(0, 360)));
		m_direction = Vector3(cos, 0, sin);
		m_direction.Normalize();
	}
	else
	{
		m_direction = Vector3(playerPosition.x - position.x, 0, playerPosition.z - position.z);
		m_direction.Normalize();
	}
}

void ButiEngine::BaseEnemy::Attack()
{
	//‹——£ŒvŽZ
	Value_weak_ptr<RigidBodyComponent> playerRigidBody = m_vwp_player.lock()->GetGameComponent<RigidBodyComponent>();
	Vector3 playerPosition = playerRigidBody.lock()->GetRigidBody()->GetPosition();
	Vector3 position = m_vwp_rigidBody.lock()->GetRigidBody()->GetPosition();
	float distance = playerPosition.Distance(position);

	if (distance <= m_minimumDistance)
	{
		m_vwp_gunComponent.lock()->ShootStart();
	}
	else
	{
		m_vwp_gunComponent.lock()->ShootStop();
	}
}

void ButiEngine::BaseEnemy::Damage(const int32_t arg_power)
{
	if (m_isInvincivle)
	{
		return;
	}

	m_isInvincivle = true;
	m_vlp_invincivleTime->Reset();
	m_hitPoint -= arg_power;

	if (m_hitPoint <= 0)
	{
		Dead();
	}
}

void ButiEngine::BaseEnemy::SetLookAtParameter()
{
	m_vwp_lookAt = m_vwp_drawObject.lock()->GetGameComponent<LookAtComponent>();
	auto drawObjectTransform = m_vwp_drawObject.lock()->transform;
	m_vwp_lookAt.lock()->SetLookTarget(drawObjectTransform->Clone());
	m_vwp_lookAt.lock()->GetLookTarget()->Translate(drawObjectTransform->GetFront() * 100.0f);
	m_vwp_lookAt.lock()->SetSpeed(0.3f);
}
