#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class RigidBodyComponent;
	class Gun;

	class LesserEnemy :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "LesserEnemy";
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
			archive(m_speed);
			archive(m_moveRate);
			archive(m_minimumDistance);
		}

		void Dead();
	private:
		void Move();
		void Rotate();
		void DecideDirection();
		void Shoot();

		void SetLookAtParameter();

		//物理挙動関連
		Value_weak_ptr<RigidBodyComponent> m_vwp_rigidBody;

		//描画用オブジェクト関連
		Value_weak_ptr<GameObject> m_vwp_drawObject;

		//銃関連
		Value_weak_ptr<GameObject> m_vwp_gun;
		Value_weak_ptr<Gun> m_vwp_gunComponent;

		//向く方向設定関連
		Value_weak_ptr<LookAtComponent> m_vwp_lookAt;

		//Player視認用
		Value_weak_ptr<GameObject> m_vwp_player;

		//移動
		Value_ptr<RelativeTimer> m_vlp_directionDicisionInterval;
		std::int32_t m_moveRate = 10;
		float m_speed = 1.0f;
		float m_minimumDistance = 3.0f;
		Vector3 m_direction;
	};

}

BUTI_REGIST_GAMECOMPONENT(LesserEnemy, true);