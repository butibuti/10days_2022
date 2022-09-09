#include "stdafx_u.h"
#include "Explosion.h"

void ButiEngine::Explosion::OnUpdate()
{
}

void ButiEngine::Explosion::OnSet()
{
}

void ButiEngine::Explosion::Start()
{
	std::string damageAreaName;
	if (gameObject.lock()->HasGameObjectTag("Explosion_Player"))
	{
		damageAreaName = "DamageArea_Player_Explosion";
	}
	else if (gameObject.lock()->HasGameObjectTag("Explosion_Enemy"))
	{
		//�G�������o�������Ƃ�������ӂ�����
		//damageAreaName = "DamageArea_Enemy_Explosion";
	}

	auto damageArea = GetManager().lock()->AddObjectFromCereal(damageAreaName);
	damageArea.lock()->transform->SetLocalPosition(gameObject.lock()->transform->GetLocalPosition());

	//�������p�[�e�B�N���Ƃ��ŕ\������Ȃ�p�[�e�B�N���o���ăI�u�W�F�N�g�폜
	//�X�N���[���A�j���[�V�����ŕ\������Ȃ烉�C�t��������
	//���͂Ƃ肠���������ō폜����
	gameObject.lock()->SetIsRemove(true);
}

void ButiEngine::Explosion::OnRemove()
{
}

void ButiEngine::Explosion::OnShowUI()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::Explosion::Clone()
{
	return ObjectFactory::Create<Explosion>();
}

void ButiEngine::Explosion::Dead()
{
}
