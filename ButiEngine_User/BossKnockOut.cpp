#include "stdafx_u.h"
#include "BossKnockOut.h"
#include "BaseItem.h"
#include "LesserEnemy.h"
#include "Bullet.h"
#include "Bullet_GrenadeLauncher.h"
#include "LargeGunItem.h"
#include "ShotGunItem.h"
#include "GrenadeLauncherItem.h"
#include "SeparateDrawObject.h"
#include "WhiteOut.h"

void ButiEngine::BossKnockOut::OnUpdate()
{
	if (m_vlp_timer->Update())
	{
		m_vlp_timer->Stop();


		SetIsRemove(true);
	}
}

void ButiEngine::BossKnockOut::OnSet()
{
	m_vlp_timer = ObjectFactory::Create<RelativeTimer>(60);
	m_vlp_timer->Start();

	auto white = GetManager().lock()->AddObjectFromCereal("White");
	white.lock()->GetGameComponent<WhiteOut>()->SetLife(m_vlp_timer->GetMaxCountFrame());

	auto drawObjectTransform = gameObject.lock()->GetGameComponent<SeparateDrawObject>()->GetDrawObject().lock()->transform;
	drawObjectTransform->SetLocalRotationY_Degrees(-130.0f);
	drawObjectTransform->RollLocalRotationZ_Degrees(90.0f);

	DeleteEnemySideObject();
}

void ButiEngine::BossKnockOut::Start()
{
}

void ButiEngine::BossKnockOut::OnRemove()
{
}

void ButiEngine::BossKnockOut::OnShowUI()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::BossKnockOut::Clone()
{
	return ObjectFactory::Create<BossKnockOut>();
}

void ButiEngine::BossKnockOut::DeleteEnemySideObject()
{
	//敵オブジェクトの消去
	auto enemies = GetManager().lock()->GetGameObjects(GameObjectTag("Enemy"));
	for (auto enemy : enemies)
	{
		if (enemy->HasGameObjectTag("BaseEnemy"))
		{
			enemy->GetGameComponent<BaseEnemy>()->Dead();
		}
		else if (enemy->HasGameObjectTag("LesserEnemy"))
		{
			enemy->GetGameComponent<LesserEnemy>()->Dead();
		}
	}

	//敵の弾の消去
	auto bullets = GetManager().lock()->GetGameObjects(GameObjectTag("Bullet_Enemy"));
	for (auto bullet : bullets)
	{
		auto bulletComponent = bullet->GetGameComponent<Bullet>();
		if (bulletComponent)
		{
			bulletComponent->Dead();
		}
		auto bulletGLComponent = bullet->GetGameComponent<Bullet_GrenadeLauncher>();
		if (bulletGLComponent)
		{
			bulletGLComponent->Dead();
		}
	}

	//アイテムの消去
	auto items = GetManager().lock()->GetGameObjects(GameObjectTag("Item"));
	for (auto item : items)
	{
		auto BaseItemComponent = item->GetGameComponent<BaseItem>();
		if (BaseItemComponent)
		{
			BaseItemComponent->Dead();

		}
		auto LargeGunItemComponent = item->GetGameComponent<LargeGunItem>();
		if (LargeGunItemComponent)
		{
			LargeGunItemComponent->Dead();

		}
		auto ShotGunItemComponent = item->GetGameComponent<ShotGunItem>();
		if (ShotGunItemComponent)
		{
			ShotGunItemComponent->Dead();

		}
		auto grenadeLauncherItemComponent = item->GetGameComponent<GrenadeLauncherItem>();
		if (grenadeLauncherItemComponent)
		{
			grenadeLauncherItemComponent->Dead();

		}
	}
}
