#include "stdafx_u.h"
#include "EnemySpawner.h"
#include "Header/GameObjects/DefaultGameComponent/RigidBodyComponent.h"

void ButiEngine::EnemySpawner::OnUpdate()
{
	if (m_isPause)
	{
		return;
	}

	if (m_vlp_spawnTimer->Update())
	{
		//Spawn
		//‹——£ŒvŽZ
		Vector3 playerPosition = GetManager().lock()->GetGameObject("Player").lock()->GetGameComponent<RigidBodyComponent>()->GetRigidBody()->GetPosition();
		float length = 20.0f;

		float sin, cos;
		MathHelper::SinCos(sin, cos, MathHelper::ToRadian(ButiRandom::GetInt(0, 360)));
		Vector3 spawnDirection = Vector3(cos, 0, sin);
		spawnDirection.Normalize();

		auto enemy = GetManager().lock()->AddObjectFromCereal("BaseEnemy");
		enemy.lock()->transform->SetLocalRotation(playerPosition + spawnDirection * length);
	}
}

void ButiEngine::EnemySpawner::OnSet()
{
}

void ButiEngine::EnemySpawner::Start()
{
	m_waveCount = 0;

	if (!m_vec_vec_emitObjectParameter.empty())
	{
		for (int i = 0; i < m_vec_vec_emitObjectParameter[0].size(); i++)
		{
			std::string name = m_vec_vec_emitObjectParameter[0][i].first;
			if (name == "BaseEnemy")
			{
				auto enemy = GetManager().lock()->AddObjectFromCereal("BaseEnemy");
				enemy.lock()->transform->SetLocalPosition(m_vec_vec_emitObjectParameter[0][i].second);
			}
			else if (name == "LesserEnemy")
			{
				auto enemy = GetManager().lock()->AddObjectFromCereal("LesserEnemy");
				enemy.lock()->transform->SetLocalPosition(m_vec_vec_emitObjectParameter[0][i].second);
			}
		}
	}

	m_spawnInterVal = 300;
	m_vlp_spawnTimer = ObjectFactory::Create<RelativeTimer>(m_spawnInterVal);
	m_vlp_spawnTimer->Start();
	m_isPause = false;

	m_vwp_playerRigidBody = GetManager().lock()->GetGameObject("Player").lock()->GetGameComponent<RigidBodyComponent>();
}

void ButiEngine::EnemySpawner::OnRemove()
{
}

void ButiEngine::EnemySpawner::OnShowUI()
{
	//Wave‚Ì’Ç‰Á
	if (GUI::Button("Add Wave"))
	{
		std::vector<std::pair<std::string, Vector3>> temp;
		m_vec_vec_emitObjectParameter.push_back(temp);
	}

	static char buff[128];
	for (int i = 0; i < m_vec_vec_emitObjectParameter.size(); i++)
	{
		GUI::Text("------------");
		GUI::BulletText(u8"Wave" + std::to_string(i));
		//Wave‚ÌÅŒã”ö‚É’Ç‰Á
		GUI::InputTextWithHint("##objectName" + std::to_string(i), m_nameHint, buff, sizeof(buff));
		if (GUI::Button("Add Object Wave" + std::to_string(i)))
		{
			m_nameHint = buff;
			m_vec_vec_emitObjectParameter[i].push_back(std::make_pair(m_nameHint, Vector3()));
		}

		//Wave“à‚Ì—v‘f‚ð—ñ‹“Eposition‚ðÝ’è
		for (int j = 0; j < m_vec_vec_emitObjectParameter[i].size(); j++)
		{
			std::string posLabel = "##position" + std::to_string(i) + "_" + std::to_string(j);
			GUI::BulletText(u8"objectName:" + m_vec_vec_emitObjectParameter[i][j].first);
			GUI::Text("position");
			GUI::DragFloat3(posLabel, m_vec_vec_emitObjectParameter[i][j].second, 1.0f, -1000.0f, 1000.0f);
		}

		if (!m_vec_vec_emitObjectParameter[i].empty())
		{
			//Wave“à‚ÌÅŒã”ö‚ðíœ
			if (GUI::Button("Delete Object Wave" + std::to_string(i)))
			{
				m_vec_vec_emitObjectParameter[i].pop_back();
			}
		}
		GUI::Text("------------");
	}

	//Wave“à‚ÌÅŒã”ö‚ðíœ
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
