#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class TitleLine;

	class TitleLineManager :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "TitleLineManager";
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

		void Appear();
		void Disappear();
	private:
		std::vector<Value_weak_ptr<TitleLine>> m_vec_rightTitleLineComponents;
		std::vector<Value_weak_ptr<TitleLine>> m_vec_leftTitleLineComponents;
		Value_ptr<RelativeTimer> m_vlp_timer;

		std::int32_t m_titleLineNum;
		bool m_isAppear;
		bool m_isDisappear;
	};

}

BUTI_REGIST_GAMECOMPONENT(TitleLineManager, true);