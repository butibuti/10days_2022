#pragma once
#include"Header/GameComponentHeader.h"

namespace ButiEngine {
	class ItemEmitParameter :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "ItemEmitParameter";
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
			archive(m_emitPercentage);
			archive(m_emitTypeRatios);
		}

		const std::string CalculateItemEmitType()const;
	private:
		std::int32_t m_emitPercentage;
		std::vector<std::pair<std::string, std::int32_t>> m_emitTypeRatios;
	};

}

BUTI_REGIST_GAMECOMPONENT(ItemEmitParameter, true);