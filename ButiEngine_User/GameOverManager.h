#pragma once
#include"Header/GameComponentHeader.h"
#include "BaseEnemy.h"
namespace ButiEngine {

	class GameOverManager :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "GameOverManager";
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

	private:
		Value_ptr<RelativeTimer> m_vlp_waitTimer;
		Value_ptr<RelativeTimer> m_vlp_timer;
	};

}

BUTI_REGIST_GAMECOMPONENT(GameOverManager, true);