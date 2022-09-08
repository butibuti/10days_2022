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

		}

		void PowerUp(const std::string& arg_gunName);
		void Dead();
	private:
		void Control();
		void Move();
		void Rotate();
		void Shoot();

		void SetLookAtParameter();

		//���������֘A
		Value_weak_ptr<RigidBodyComponent> m_vwp_rigidBody;

		//�`��p�I�u�W�F�N�g�֘A
		Value_weak_ptr<GameObject> m_vwp_drawObject;

		//�e�֘A
		Value_weak_ptr<Gun> m_vwp_gunComponent;
		Value_weak_ptr<EquipGun> m_vwp_equipGunComponent;

		//�v���C���[�̌��������ݒ�֘A
		Value_weak_ptr<LookAtComponent> m_vwp_lookAt;
	};

}

BUTI_REGIST_GAMECOMPONENT(Player, true);