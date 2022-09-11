#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {
	class Gun :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "Gun";
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
			archive(m_diffusion);
			archive(m_range);
			archive(m_power);
			archive(m_shootIntervalFrame);
			archive(m_bulletName);
			archive(m_bulletSpeed);
			archive(m_bulletCount);
			archive(m_offset);
			archive(m_vlp_recoilTransform);
		}

		void SetOwner(Value_weak_ptr<GameObject> arg_owner) { m_vwp_owner = arg_owner; }

		std::int32_t GetShootIntervalFrame() { return m_shootIntervalFrame; }

		void ShootStart();
		void ShootStop();
	private:
		void Shoot();
		void SetNormalBulletParameter(Value_weak_ptr<GameObject> arg_bullet);
		void SetGrenadeLauncherBulletParameter(Value_weak_ptr<GameObject> arg_bullet);
		void Recoil();


		Value_weak_ptr<GameObject> m_vwp_owner;

		Value_ptr<RelativeTimer> m_vlp_shootIntervalTimer;
		bool m_isShoot;

		//ägéU
		float m_diffusion = 10.0f;
		//éÀíˆ
		float m_range = 10.0f;
		//à–óÕ
		float m_power = 1.0f;
		//òAéÀë¨ìx
		std::int32_t m_shootIntervalFrame = 10;
		//íeÇÃñºëO
		std::string m_bulletName = "Bullet_Player_Normal";
		//íeë¨
		float m_bulletSpeed = 0.2f;
		//àÍâÒÇ…î≠éÀÇ≥ÇÍÇÈíeÇÃêî
		std::int32_t m_bulletCount = 3;
		//íeÇ™ê∂ê¨Ç≥ÇÍÇÈà íu
		Vector3 m_offset = Vector3Const::Zero;
		//îΩìÆå„Transform
		Value_ptr<Transform> m_vlp_recoilTransform;
	};

}

BUTI_REGIST_GAMECOMPONENT(Gun, true);