#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {
	class TriggerComponent;
	class Player;

	class BaseItem :public GameComponent
	{
	public:
		virtual std::string GetGameComponentName()const override {
			return "BaseItem";
		}
		virtual void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		void OnRemove()override;
		virtual void OnShowUI()override;
		virtual Value_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);

		}

		virtual void PowerUpPlayer(Value_weak_ptr<Player> arg_vwp_player);
		virtual void Dead();
	protected:
		Value_weak_ptr<TriggerComponent> m_vwp_trigger;

		bool m_isHitInCurrentFrame;
	};

}

BUTI_REGIST_GAMECOMPONENT(BaseItem, true);