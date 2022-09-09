#include "stdafx_u.h"
#include "Bullet.h"
#include "ButiBulletWrap/ButiBulletWrap/Common.h"
#include "Header/GameObjects/DefaultGameComponent/RigidBodyComponent.h"
#include "SeparateDrawObject.h"

void ButiEngine::Bullet::OnUpdate()
{
	gameObject.lock()->transform->Translate(m_velocity * GameDevice::WorldSpeed);

	//射程範囲を超えたら死ぬ
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
	//他のオブジェクトと当たったら死ぬ
	gameObject.lock()->AddCollisionEnterReaction(
		[this](ButiBullet::ContactData& arg_other)
		{
			if (arg_other.vwp_gameObject.lock())
			{
				if (arg_other.vwp_gameObject.lock() != m_vwp_owner.lock())
				{
					Dead();
				}
			}
		}
	);
}

void ButiEngine::Bullet::Start()
{
	m_startPos = gameObject.lock()->transform->GetLocalPosition();
}

void ButiEngine::Bullet::OnRemove()
{
}

void ButiEngine::Bullet::OnShowUI()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Bullet::Clone()
{
	return ObjectFactory::Create<Bullet>();
}

void ButiEngine::Bullet::Dead()
{
	gameObject.lock()->GetGameComponent<SeparateDrawObject>()->Dead();
	gameObject.lock()->SetIsRemove(true);
}
