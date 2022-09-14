#include "stdafx_u.h"
#include "EnemySpawner.h"
#include "Header/GameObjects/DefaultGameComponent/RigidBodyComponent.h"

void ButiEngine::EnemySpawner::OnUpdate()
{
	if (m_isPause)
	{
		return;
	}

	AdvanceTimeSpawnEnemy();

	if (!m_isFinishTutorial)
	{
		return;
	}

	if (m_vlp_spawnTimer->Update())
	{
		//Spawn
		//ãóó£åvéZ
		Vector3 playerPosition = GetManager().lock()->GetGameObject("Player").lock()->GetGameComponent<RigidBodyComponent>()->GetRigidBody()->GetPosition();
		float length = 20.0f;

		float sin, cos;
		MathHelper::SinCos(sin, cos, MathHelper::ToRadian(ButiRandom::GetInt(0, 360)));
		Vector3 spawnDirection = Vector3(cos, 0, sin);
		spawnDirection.Normalize();

		Vector3 spawnPosition = playerPosition + spawnDirection * length;
		RegistSpawnEnemy("LesserEnemy", spawnPosition);
		AddObjectFromCereal("LineEffect_LesserEnemy", ObjectFactory::Create<Transform>(spawnPosition));
	}
}

void ButiEngine::EnemySpawner::OnSet()
{
}

void ButiEngine::EnemySpawner::Start()
{
	m_waveCount = 0;
	m_vec_spawnEnemy.clear();

	/*if (!m_vec_vec_emitObjectParameter.empty())
	{
		for (int i = 0; i < m_vec_vec_emitObjectParameter[0].size(); i++)
		{
			RegistSpawnEnemy(m_vec_vec_emitObjectParameter[0][i].first, m_vec_vec_emitObjectParameter[0][i].second);
		}
	}*/
	RegistSpawnEnemy("TutorialEnemy", Vector3(0, 0, 7));

	m_spawnInterVal = 300;
	m_vlp_spawnTimer = ObjectFactory::Create<RelativeTimer>(m_spawnInterVal);
	m_isPause = false;
	m_isFinishTutorial = false;

	m_vwp_playerRigidBody = GetManager().lock()->GetGameObject("Player").lock()->GetGameComponent<RigidBodyComponent>();
}

void ButiEngine::EnemySpawner::OnRemove()
{
}

void ButiEngine::EnemySpawner::OnShowUI()
{
	static char buff[128];
	GUI::BulletText(u8"EnemyÇÃê∂ê¨óPó\éûä‘");
	GUI::InputTextWithHint("##beforeSpawnEnemyTime", m_nameHint, buff, sizeof(buff));
	if (GUI::Button("Add beforeSpawnEnemyTime"))
	{
		m_nameHint = buff;
		m_map_beforeSpawnEnemyTime.emplace(m_nameHint, 60);
	}
	for (auto element : m_map_beforeSpawnEnemyTime)
	{
		GUI::BulletText(u8"" + element.first);
		GUI::DragInt("##time_" + element.first, &m_map_beforeSpawnEnemyTime[element.first], 1.0f, 0, 1000);
	}
	GUI::Text("------------");

	//WaveÇÃí«â¡
	if (GUI::Button("Add Wave"))
	{
		std::vector<std::pair<std::string, Vector3>> temp;
		m_vec_vec_emitObjectParameter.push_back(temp);
	}

	for (int i = 0; i < m_vec_vec_emitObjectParameter.size(); i++)
	{
		GUI::Text("------------");
		GUI::BulletText(u8"Wave" + std::to_string(i));
		//WaveÇÃç≈å„îˆÇ…í«â¡
		GUI::InputTextWithHint("##objectName" + std::to_string(i), m_nameHint, buff, sizeof(buff));
		if (GUI::Button("Add Object Wave" + std::to_string(i)))
		{
			m_nameHint = buff;
			m_vec_vec_emitObjectParameter[i].push_back(std::make_pair(m_nameHint, Vector3()));
		}

		//Waveì‡ÇÃóvëfÇóÒãìÅEpositionÇê›íË
		for (int j = 0; j < m_vec_vec_emitObjectParameter[i].size(); j++)
		{
			std::string posLabel = "##position" + std::to_string(i) + "_" + std::to_string(j);
			GUI::BulletText(u8"objectName:" + m_vec_vec_emitObjectParameter[i][j].first);
			GUI::Text("position");
			GUI::DragFloat3(posLabel, m_vec_vec_emitObjectParameter[i][j].second, 1.0f, -1000.0f, 1000.0f);
		}

		if (!m_vec_vec_emitObjectParameter[i].empty())
		{
			//Waveì‡ÇÃç≈å„îˆÇçÌèú
			if (GUI::Button("Delete Object Wave" + std::to_string(i)))
			{
				m_vec_vec_emitObjectParameter[i].pop_back();
			}
		}
		GUI::Text("------------");
	}

	//Waveì‡ÇÃç≈å„îˆÇçÌèú
	if (!m_vec_vec_emitObjectParameter.empty())
	{
		if (GUI::Button("Delete Wave"))
		{
			m_vec_vec_emitObjectParameter.pop_back();
		}
	}
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::EnemySpawner::Clone()
{
	auto output = ObjectFactory::Create<EnemySpawner>();
	output->m_vec_vec_emitObjectParameter = m_vec_vec_emitObjectParameter;
	return output;
}

void ButiEngine::EnemySpawner::FinishTutorial()
{
	if (!m_vec_vec_emitObjectParameter.empty())
	{
		for (int i = 0; i < m_vec_vec_emitObjectParameter[0].size(); i++)
		{
			RegistSpawnEnemy(m_vec_vec_emitObjectParameter[0][i].first, m_vec_vec_emitObjectParameter[0][i].second);
		}
	}

	m_vlp_spawnTimer->Start();
	m_isFinishTutorial = true;
}

void ButiEngine::EnemySpawner::StartPause()
{
	m_isPause = true;
	m_vlp_spawnTimer->Stop();
}

void ButiEngine::EnemySpawner::FinishPause()
{
	m_isPause = false;
	m_vlp_spawnTimer->Start();
}

void ButiEngine::EnemySpawner::RegistSpawnEnemy(const std::string& arg_enemyName, const Vector3& arg_position)
{
	auto itr = m_map_beforeSpawnEnemyTime.find(arg_enemyName);
	if (itr != m_map_beforeSpawnEnemyTime.end())
	{
		m_vec_spawnEnemy.push_back(std::make_tuple(arg_enemyName, arg_position, itr->second));
	}
	else
	{
		m_vec_spawnEnemy.push_back(std::make_tuple(arg_enemyName, arg_position, 60));
	}
}

void ButiEngine::EnemySpawner::AdvanceTimeSpawnEnemy()
{
	for (auto itr = m_vec_spawnEnemy.begin(); itr != m_vec_spawnEnemy.end();)
	{
		std::int32_t& time = std::get<2>(*itr);
		time--;

		//ê∂ê¨
		if (time <= 0)
		{
			std::string& name = std::get<0>(*itr);
			Vector3& position = std::get<1>(*itr);

			if (name == "BaseEnemy")
			{
				auto enemy = GetManager().lock()->AddObjectFromCereal("BaseEnemy");
				enemy.lock()->transform->SetLocalPosition(position);
			}
			else if (name == "LesserEnemy")
			{
				auto enemy = GetManager().lock()->AddObjectFromCereal("LesserEnemy");
				enemy.lock()->transform->SetLocalPosition(position);
			}
			else if (name == "BossEnemy")
			{
				auto enemy = GetManager().lock()->AddObjectFromCereal("BossEnemy");
				enemy.lock()->transform->SetLocalPosition(position);
			}
			else if (name == "TutorialEnemy")
			{
				auto enemy = GetManager().lock()->AddObjectFromCereal("TutorialEnemy");
				enemy.lock()->transform->SetLocalPosition(position);
			}

			itr = m_vec_spawnEnemy.erase(itr);
		}
		else
		{
			itr++;
		}
	}
}
