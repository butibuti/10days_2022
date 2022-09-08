#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class RigidBodyComponent;
	class Player;

	class BaseItem :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "BaseItem";
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

		virtual void PowerUpPlayer(Value_weak_ptr<Player> arg_vwp_player);
		void Dead();
	private:
		Value_weak_ptr<RigidBodyComponent> m_vwp_rigidBody;
	};

}

BUTI_REGIST_GAMECOMPONENT(BaseItem, true);