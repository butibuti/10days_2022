#pragma once
#include"Header/GameComponentHeader.h"
#include "BaseItem.h"
namespace ButiEngine {
	class Player;

	class GrenadeLauncherItem :public BaseItem
	{
	public:
		std::string GetGameComponentName()const override {
			return "GrenadeLauncherItem";
		}
		void OnUpdate()override;
		void OnShowUI()override;
		Value_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);

		}

		void PowerUpPlayer(Value_weak_ptr<Player> arg_vwp_player)override;
		void Dead()override;
	private:
	};

}

BUTI_REGIST_GAMECOMPONENT(GrenadeLauncherItem, true);