#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Door :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "Door";
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
			archive(m_nextSceneName);
		}
	private:
		std::string m_nextSceneName;
	};

}

BUTI_REGIST_GAMECOMPONENT(Door, true);