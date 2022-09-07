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
		}

		void Dead();
	private:
		void Move();
		void DecideDirection();
		void Shoot();

		void CreateGun();

		//���������֘A
		Value_weak_ptr<RigidBodyComponent> m_vwp_rigidBody;

		//�`��p�I�u�W�F�N�g�֘A
		Value_weak_ptr<Transform> m_vwp_drawObjectTransform;

		//�e�֘A
		Value_weak_ptr<GameObject> m_vwp_gun;
		Value_weak_ptr<Gun> m_vwp_gunComponent;

		//Player���F�p
		Value_weak_ptr<GameObject> m_vwp_player;

		//�ړ�
		Value_ptr<RelativeTimer> m_vlp_directionDicisionInterval;
		std::int32_t m_moveRate = 10;
		float m_speed = 1.0f;
		Vector3 m_direction;
	};

}

BUTI_REGIST_GAMECOMPONENT(LesserEnemy, true);