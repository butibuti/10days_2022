#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class ChangeSceneArea :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "ChangeSceneArea";
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
		void ChangeScene();


		std::string m_nextSceneName;
		bool m_isHitPlayer;
	};

}

BUTI_REGIST_GAMECOMPONENT(ChangeSceneArea, true);