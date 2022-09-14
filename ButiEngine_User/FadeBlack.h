#pragma once
#include"Header/GameComponentHeader.h"
#include "BaseEnemy.h"
namespace ButiEngine {

	class FadeBlack :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "FadeBlack";
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
		Value_ptr<RelativeTimer> m_vlp_timer;
		Value_weak_ptr<MeshDrawComponent> m_vwp_meshDrawComponent;
		float m_alpha;
	};

}

BUTI_REGIST_GAMECOMPONENT(FadeBlack, true);