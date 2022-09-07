#include "stdafx_u.h"
#include "BaseItem.h"
#include "ButiBulletWrap/ButiBulletWrap/Common.h"
#include "Header/GameObjects/DefaultGameComponent/RigidBodyComponent.h"
#include "Player.h"

void ButiEngine::BaseItem::OnUpdate()
{
}

void ButiEngine::BaseItem::OnSet()
{
	//���̃I�u�W�F�N�g�Ɠ��������玀��
	gameObject.lock()->AddCollisionEnterReaction(
		[this](ButiBullet::ContactData& arg_other)
		{
			Value_weak_ptr<GameObject> vwp_obj = arg_other.vwp_gameObject;
			if (vwp_obj.lock() && vwp_obj.lock()->GetGameObjectName() == "Player")
			{
				//�h���N���X��Player�̌Ăяo�������֐����w�肷��
				Dead();
			}
		}
	);
}

void ButiEngine::BaseItem::Start()
{
	m_vwp_rigidBody = gameObject.lock()->GetGameComponent<RigidBodyComponent>();
}

void ButiEngine::BaseItem::OnRemove()
{
}

void ButiEngine::BaseItem::OnShowUI()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::BaseItem::Clone()
{
	return ObjectFactory::Create<BaseItem>();
}

void ButiEngine::BaseItem::Dead()
{
	gameObject.lock()->SetIsRemove(true);
}
