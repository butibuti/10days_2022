#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class RigidBodyComponent;
	class Gun;
	class EquipGun;

	class Player :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "Player";
		}
		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		void OnRemove()override;
		void OnShowUI()override;
		Value_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
			archive(m_maxHitPoint);
			archive(m_invincibleInterval);
		}

		void SetIsInvincible(const bool arg_isInvincible) { m_isInvincible = arg_isInvincible; }
		void SetCanAcceptInput(const bool arg_canAcceptInput) { m_canAcceptInput = arg_canAcceptInput; }

		void PowerUp(const std::string& arg_gunName);
		void Dead();
		Value_weak_ptr<Gun> ChangeGun(const std::string& arg_gunName);
		void EquipAssaultRifle();
	private:
		void Control();
		void Move();
		void Rotate();
		void Shoot();
		void Damage(const int32_t arg_power);

		void SetLookAtParameter();

		//物理挙動関連
		Value_weak_ptr<RigidBodyComponent> m_vwp_rigidBody;

		//描画用オブジェクト関連
		Value_weak_ptr<GameObject> m_vwp_drawObject;

		//銃関連
		Value_weak_ptr<Gun> m_vwp_gunComponent;
		Value_weak_ptr<EquipGun> m_vwp_equipGunComponent;

		//プレイヤーの向く方向設定関連
		Value_weak_ptr<LookAtComponent> m_vwp_lookAt;

		//体力管理
		std::int32_t m_hitPoint = 10;
		std::int32_t m_maxHitPoint = 10;
		Value_ptr<RelativeTimer> m_vlp_invincibleTime;
		std::int32_t m_invincibleInterval = 20;
		bool m_isInvincible;

		bool m_canAcceptInput;
	};

}

BUTI_REGIST_GAMECOMPONENT(Player, true);