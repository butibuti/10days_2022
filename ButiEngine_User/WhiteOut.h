#pragma once
#include"Header/GameComponentHeader.h"
#include "BaseEnemy.h"
namespace ButiEngine {

	class WhiteOut :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "WhiteOut";
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

		void SetLife(const std::int32_t arg_life) { m_life = arg_life; }
	private:
		Value_ptr<RelativeTimer> m_vlp_timer;
		Value_weak_ptr<MeshDrawComponent> m_vwp_meshDrawComponent;
		std::int32_t m_life;
		float m_alpha;
	};

}

BUTI_REGIST_GAMECOMPONENT(WhiteOut, true);