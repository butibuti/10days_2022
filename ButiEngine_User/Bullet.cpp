#include "stdafx_u.h"
#include "Bullet.h"
#include "ButiBulletWrap/ButiBulletWrap/Common.h"
#include "Header/GameObjects/DefaultGameComponent/RigidBodyComponent.h"
#include "SeparateDrawObject.h"

void ButiEngine::Bullet::OnUpdate()
{
	m_isHitInCurrentFrame = false;

	//�Z�������͓����Ȃ�
	if (m_isPause)
	{
		return;
	}

	gameObject.lock()->transform->Translate(m_velocity * GameDevice::GetWorldSpeed());

	//�˒��͈͂𒴂����玀��
	Vector3 pos = gameObject.lock()->transform->GetLocalPosition();
	float distanceSqr = (pos - m_startPos).GetLengthSqr();
	float rangeSqr = m_range * m_range;
	if (distanceSqr > rangeSqr)
	{
		Dead();
	}
}

void ButiEngine::Bullet::OnSet()
{
	gameObject.lock()->AddCollisionEnterReaction(
		[this](ButiBullet::ContactData& arg_other)
		{
			if (arg_other.vwp_gameObject.lock())
			{
				if (arg_other.vwp_gameObject.lock() != m_vwp_owner.lock() && !m_isHitInCurrentFrame)
				{
					m_isHitInCurrentFrame = true;
					AddHitObject(arg_other.vwp_gameObject);

					//���������I�u�W�F�N�g���ђʉ\�񐔂�葽�������玀��
					if (m_vec_hitObjects.size() > m_penetratingCount)
					{
						Dead();
					}
				}
			}
		}
	);
}

void ButiEngine::Bullet::Start()
{
	m_startPos = gameObject.lock()->transform->GetLocalPosition();
	m_isHitInCurrentFrame = false;

	m_vec_hitObjects.clear();

	m_isPause = false;
	m_beforePauseVelocity = Vector3();
	if (StringHelper::Contains(gameObject.lock()->GetGameObjectName(), "Bullet_Player_SpecialGrenadeLauncher"))
	{
		PlaySE("Sound/ShootLauncher.wav", 0.1f);
	}
	else if (StringHelper::Contains(gameObject.lock()->GetGameObjectName(), "Bullet_Player_Shotgun"))
	{
		PlaySE("Sound/ShotGun.wav", 0.02f);
	}
	else if (StringHelper::Contains(gameObject.lock()->GetGameObjectName(), "Bullet_Player_Normal"))
	{
		PlaySE("Sound/Pistol.wav", 0.1f);
	}
	else if (StringHelper::Contains(gameObject.lock()->GetGameObjectName(), "Bullet_Player_Large"))
	{
		PlaySE("Sound/AssaultRifle.wav", 0.1f);
	}
	else if (StringHelper::Contains(gameObject.lock()->GetGameObjectName(), "Bullet_Player_GrenadeLauncher"))
	{
		PlaySE("Sound/ShootLauncher.wav", 0.1f);
	}
	else if (StringHelper::Contains(gameObject.lock()->GetGameObjectName(), "Bullet_Enemy_Normal"))
	{
		PlaySE("Sound/Pistol.wav", 0.1f);
	}
	else if (StringHelper::Contains(gameObject.lock()->GetGameObjectName(), "Bullet_Boss_Normal"))
	{
		PlaySE("Sound/ShotGun.wav", 0.02f);
	}
	else if (StringHelper::Contains(gameObject.lock()->GetGameObjectName(), "Bullet_Boss_GrenadeLauncher"))
	{
		PlaySE("Sound/ShootLauncher.wav", 0.1f);
	}
}

void ButiEngine::Bullet::OnRemove()
{
}

void ButiEngine::Bullet::OnShowUI()
{
	GUI::BulletText(u8"�ђʂł���I�u�W�F�N�g�̐�");
	GUI::DragInt("##penetratingCount", &m_penetratingCount, 1.0f, 0, 100);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Bullet::Clone()
{
	auto output = ObjectFactory::Create<Bullet>();
	output->m_penetratingCount = m_penetratingCount;
	return output;
}

void ButiEngine::Bullet::Dead()
{
	gameObject.lock()->GetGameComponent<SeparateDrawObject>()->Dead();
	gameObject.lock()->SetIsRemove(true);
}

void ButiEngine::Bullet::StartPause()
{
	m_isPause = true;
	m_beforePauseVelocity = m_velocity;
}

void ButiEngine::Bullet::FinishPause()
{
	if (!m_isPause)
	{
		return;
	}

	m_isPause = false;
	m_velocity = m_beforePauseVelocity;
}

void ButiEngine::Bullet::AddHitObject(Value_weak_ptr<GameObject> arg_vwp_hitObject)
{
	//���ɂɒǉ�����Ă�����return
	auto find = std::find(m_vec_hitObjects.begin(), m_vec_hitObjects.end(), arg_vwp_hitObject);
	if (find != m_vec_hitObjects.end())
	{
		return;
	}

	m_vec_hitObjects.push_back(arg_vwp_hitObject);
}
