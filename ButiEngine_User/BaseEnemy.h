#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class RigidBodyComponent;

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
			archive(m_invincivleInterval);
			archive(m_attackInterval);
		}

		virtual void Dead();
	protected:
		virtual void Control();
		virtual void Move();
		virtual void Rotate();
		virtual void DecideDirection();
		virtual void Attack();
		virtual void Damage(const int32_t arg_power);

		virtual void SetLookAtParameter();

		//���������֘A
		Value_weak_ptr<RigidBodyComponent> m_vwp_rigidBody;

		//�`��p�I�u�W�F�N�g�֘A
		Value_weak_ptr<GameObject> m_vwp_drawObject;

		//���������ݒ�֘A
		Value_weak_ptr<LookAtComponent> m_vwp_lookAt;

		//Player���F�p
		Value_weak_ptr<GameObject> m_vwp_player;

		//�ړ�
		Value_ptr<RelativeTimer> m_vlp_directionDicisionTime;
		std::int32_t m_directionDicisionInterval = 10;
		float m_speed = 1.0f;
		float m_minimumDistance = 3.0f;
		Vector3 m_direction;

		//�̗͊Ǘ�
		std::int32_t m_hitPoint = 10;
		std::int32_t m_maxHitPoint = 10;
		Value_ptr<RelativeTimer> m_vlp_invincivleTime;
		std::int32_t m_invincivleInterval = 20;
		bool m_isInvincivle;

		//�U���֘A
		Value_ptr<RelativeTimer> m_vlp_attackTime;
		std::int32_t m_attackInterval = 60;
	};

}

BUTI_REGIST_GAMECOMPONENT(BaseEnemy, true);