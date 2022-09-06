#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Bullet :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "Bullet";
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
	};

}

BUTI_REGIST_GAMECOMPONENT(Bullet, true);