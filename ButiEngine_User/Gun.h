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
			archive(m_rate);
			archive(m_bulletName);
			archive(m_bulletSpeed);
			archive(m_bulletCount);
			archive(m_offset);
		}

		void SetIsShoot(const bool arg_isShoot);

		void Shoot();
	private:
		Value_ptr<RelativeTimer> m_vlp_shootInterval;
		bool m_isShoot;

		//ŠgU
		float m_diffusion = 10.0f;
		//Ë’ö
		float m_range = 10.0f;
		//ˆĞ—Í
		float m_power = 1.0f;
		//˜AË‘¬“x
		std::int32_t m_rate = 10;
		//’e‚Ì–¼‘O
		std::string m_bulletName = "Bullet_Player_Normal";
		//’e‘¬
		float m_bulletSpeed = 10.0f;
		//ˆê‰ñ‚É”­Ë‚³‚ê‚é’e‚Ì”
		std::int32_t m_bulletCount = 3;
		//’e‚ª¶¬‚³‚ê‚éˆÊ’u
		Vector3 m_offset = Vector3Const::Zero;
	};

}

BUTI_REGIST_GAMECOMPONENT(Gun, true);