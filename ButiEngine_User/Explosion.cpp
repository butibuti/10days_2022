#include "stdafx_u.h"
#include "Explosion.h"
#include "Camera.h"

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
		//敵が爆発出したいときここら辺いじる
		damageAreaName = "DamageArea_Enemy_Explosion";
	}

	auto damageArea = GetManager().lock()->AddObjectFromCereal(damageAreaName);
	damageArea.lock()->transform->SetLocalPosition(gameObject.lock()->transform->GetLocalPosition());

	//爆発をパーティクルとかで表現するならパーティクル出してオブジェクト削除
	//スクロールアニメーションで表現するならライフ持たせる
	//今はとりあえずここで削除する
	gameObject.lock()->SetIsRemove(true);

	GetManager().lock()->GetGameObject("CameraMan").lock()->GetGameComponent<Camera>()->StartShake(5, Vector3(1.0f, 1.0f, 0.0f), 3);

	PlaySE("Sound/Bomb.wav", 0.1f);
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
