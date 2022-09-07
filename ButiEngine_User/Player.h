#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class RigidBodyComponent;
	class Gun;

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

		void Dead();
	private:
		void Move();
		void Shoot();

		//物理挙動関連
		Value_weak_ptr<RigidBodyComponent> m_vwp_rigidBody;

		//描画用オブジェクト関連
		Value_weak_ptr<Transform> m_vwp_drawObjectTransform;

		//銃関連
		Value_weak_ptr<GameObject> m_vwp_gun;
		Value_weak_ptr<Gun> m_vwp_gunComponent;
	};

}

BUTI_REGIST_GAMECOMPONENT(Player, true);