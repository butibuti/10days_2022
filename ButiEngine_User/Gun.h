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
			archive(m_isRandomShoot);
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
		Vector3 GetOffset() { return m_offset; }

		void ShootStart();
		void ShootStop();
		void Dead();
	private:
		void Shoot();
		void SetNormalBulletParameter(Value_weak_ptr<GameObject> arg_bullet);
		void SetGrenadeLauncherBulletParameter(Value_weak_ptr<GameObject> arg_bullet);
		void Recoil();

		Value_ptr<RelativeTimer> m_vlp_deadTimer;
		Vector3 m_velocity;

		Value_weak_ptr<GameObject> m_vwp_owner;

		Value_ptr<RelativeTimer> m_vlp_shootIntervalTimer;
		bool m_isShoot;

		//’e‚ª“™ŠÔŠu‚Å”­Ë‚³‚ê‚é‚©
		bool m_isRandomShoot = true;
		//ŠgU
		float m_diffusion = 10.0f;
		//Ë’ö
		float m_range = 10.0f;
		//ˆĞ—Í
		float m_power = 1.0f;
		//˜AË‘¬“x
		std::int32_t m_shootIntervalFrame = 10;
		//’e‚Ì–¼‘O
		std::string m_bulletName = "Bullet_Player_Normal";
		//’e‘¬
		float m_bulletSpeed = 0.2f;
		//ˆê‰ñ‚É”­Ë‚³‚ê‚é’e‚Ì”
		std::int32_t m_bulletCount = 3;
		//’e‚ª¶¬‚³‚ê‚éˆÊ’u
		Vector3 m_offset = Vector3Const::Zero;
		//”½“®ŒãTransform
		Value_ptr<Transform> m_vlp_recoilTransform;
	};

}

BUTI_REGIST_GAMECOMPONENT(Gun, true);