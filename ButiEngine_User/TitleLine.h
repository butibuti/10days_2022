#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {
	class TitleLine :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "TitleLine";
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
		void AddPositionAnimation(const Vector3& arg_targetPos, const Easing::EasingType arg_easeType);

		Vector3 m_startPos;
		Vector3 m_targetPos;
	};

}

BUTI_REGIST_GAMECOMPONENT(TitleLine, true);