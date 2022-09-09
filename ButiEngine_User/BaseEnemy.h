#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class RigidBodyComponent;
	class Gun;
	class EquipGun;

	class BaseEnemy :public GameComponent
	{
	public:

		virtual std::string GetGameComponentName()const override {
			return "BaseEnemy";
		}
		virtual void OnUpdate()override;
		virtual void OnSet()override;
		virtual void Start()override;
		virtual void OnRemove()override;
		virtual void OnShowUI()override;
		virtual Value_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
			archive(m_speed);
			archive(m_directionDicisionInterval);
			archive(m_minimumDistance);
			archive(m_maxHitPoint);
			archive(m_invincibleInterval);
			archive(m_attackInterval);
		}

		virtual void Dead();
		virtual Value_weak_ptr<Gun> ChangeGun(const std::string& arg_gunName);
	protected:
		virtual void Control();
		virtual void Move();
		virtual void Rotate();
		virtual void DecideDirection();
		virtual void Attack();
		virtual void Damage(const int32_t arg_power);

		virtual void SetLookAtParameter();

		//物理挙動関連
		Value_weak_ptr<RigidBodyComponent> m_vwp_rigidBody;

		//描画用オブジェクト関連
		Value_weak_ptr<GameObject> m_vwp_drawObject;

		//銃関連
		Value_weak_ptr<Gun> m_vwp_gunComponent;
		Value_weak_ptr<EquipGun> m_vwp_equipGunComponent;

		//向く方向設定関連
		Value_weak_ptr<LookAtComponent> m_vwp_lookAt;

		//Player視認用
		Value_weak_ptr<GameObject> m_vwp_player;

		//移動
		Value_ptr<RelativeTimer> m_vlp_directionDicisionTime;
		std::int32_t m_directionDicisionInterval = 10;
		float m_speed = 1.0f;
		float m_minimumDistance = 3.0f;
		Vector3 m_direction;

		//体力管理
		std::int32_t m_hitPoint = 10;
		std::int32_t m_maxHitPoint = 10;
		Value_ptr<RelativeTimer> m_vlp_invincibleTime;
		std::int32_t m_invincibleInterval = 20;
		bool m_isInvincible;

		//攻撃関連
		Value_ptr<RelativeTimer> m_vlp_attackTime;
		std::int32_t m_attackInterval = 60;
	};

}

BUTI_REGIST_GAMECOMPONENT(BaseEnemy, true);