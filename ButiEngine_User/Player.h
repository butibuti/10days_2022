#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class RigidBodyComponent;

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


		Value_weak_ptr<RigidBodyComponent> m_vwp_rigidBody;

		//DrawObject
		Value_weak_ptr<Transform> m_vwp_drawObjectTransform;
	};

}

BUTI_REGIST_GAMECOMPONENT(Player, true);