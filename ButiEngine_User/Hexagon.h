#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {
	class Hexagon :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "Hexagon";
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
		void AddPositionAnimation();

		float m_moveSpeed;
		float m_rotationAngle;
		bool m_isDisappear;
	};

}

BUTI_REGIST_GAMECOMPONENT(Hexagon, true);