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

		void Appear();
		void Disappear();
	private:
		void AddPositionAnimation();
		void AddScaleAnimation();

		float m_moveSpeed;
		float m_rotationAngle;
		Vector3 m_startScale;
	};

}

BUTI_REGIST_GAMECOMPONENT(Hexagon, true);