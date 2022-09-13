#pragma once
#include"Header/GameComponentHeader.h"

namespace ButiEngine {
	class RigidBodyComponent;

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
			archive(m_map_beforeSpawnEnemyTime);
		}

		void StartPause();
		void FinishPause();

	private:
		void RegistSpawnEnemy(const std::string& arg_enemyName, const Vector3& arg_position);
		void AdvanceTimeSpawnEnemy();

		std::int32_t m_waveCount;
		std::string m_nameHint;
		std::unordered_map<std::string, std::int32_t> m_map_beforeSpawnEnemyTime;
		std::vector<std::vector<std::pair<std::string, Vector3>>> m_vec_vec_emitObjectParameter;

		std::vector<std::tuple<std::string, Vector3, std::int32_t>> m_vec_spawnEnemy;
		Value_ptr<RelativeTimer> m_vlp_spawnTimer;
		std::int32_t m_spawnInterVal;
		bool m_isPause;

		Value_weak_ptr<RigidBodyComponent> m_vwp_playerRigidBody;
	};

}

BUTI_REGIST_GAMECOMPONENT(EnemySpawner, true);