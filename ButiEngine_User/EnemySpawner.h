#pragma once
#include"Header/GameComponentHeader.h"

namespace ButiEngine {
	class EnemySpawner :public GameComponent
	{
	public:

		std::string GetGameComponentName()const override {
			return "EnemySpawner";
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
			archive(m_vec_vec_emitObjectParameter);
		}

	private:
		std::int32_t m_waveCount;
		std::string m_nameHint;
		std::vector<std::vector<std::pair<std::string, Vector3>>> m_vec_vec_emitObjectParameter;
	};

}

BUTI_REGIST_GAMECOMPONENT(EnemySpawner, true);