#include "ItemEmitParameter.h"

void ButiEngine::ItemEmitParameter::OnUpdate()
{
}

void ButiEngine::ItemEmitParameter::OnSet()
{
}

void ButiEngine::ItemEmitParameter::Start()
{
}

void ButiEngine::ItemEmitParameter::OnRemove()
{
}

void ButiEngine::ItemEmitParameter::OnShowUI()
{
	GUI::BulletText(u8"全体の生成確率%");
	GUI::DragInt("##emitPercentage", &m_emitPercentage, 1.0f, 0, 100);

	if (GUI::Button("Add"))
	{
		m_emitTypeRatios.push_back(std::make_pair<std::string, std::int32_t>("NULL", 0));
	}
	
	static char itemNameBuff[128];
	for (int i = 0; i < m_emitTypeRatios.size(); i++)
	{
		GUI::BulletText(u8"" + std::to_string(i) + "番");
		GUI::BulletText(u8"名前：" + m_emitTypeRatios[i].first);
		GUI::InputText("##itemName_" + std::to_string(i), itemNameBuff, sizeof(itemNameBuff));
		if (GUI::Button("Set" + std::to_string(i)))
		{
			m_emitTypeRatios[i].first = itemNameBuff;
			memset(itemNameBuff, 0, 128);
		}
		GUI::BulletText(u8"生成比率");
		GUI::DragInt("##emitRatio_" + std::to_string(i), &m_emitTypeRatios[i].second, 1.0f, 0, 100);
	}

	static std::string answer;
	if (GUI::Button("Calc"))
	{
		answer = CalculateItemEmitType();
	}
	GUI::Text(answer);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::ItemEmitParameter::Clone()
{
	auto output = ObjectFactory::Create<ItemEmitParameter>();
	output->m_emitPercentage = m_emitPercentage;
	output->m_emitTypeRatios = m_emitTypeRatios;
	return output;
}

const std::string ButiEngine::ItemEmitParameter::CalculateItemEmitType() const
{
	if (m_emitPercentage <= 0)
	{
		return "NoEmit";
	}
	if (m_emitTypeRatios.empty())
	{
		return "NoEmit";
	}

	//生成するかどうかを決める
	std::int32_t rnd = ButiRandom::GetInt(1, 100);
	if (rnd > m_emitPercentage)
	{
		return "NoEmit";
	}

	//何を生成するかを決める
	std::int32_t sumRatios = 0;
	for (int i = 0; i < m_emitTypeRatios.size(); i++)
	{
		sumRatios += m_emitTypeRatios[i].second;
	}
	if (sumRatios <= 0)
	{
		return "NoEmit";
	}

	rnd = ButiRandom::GetInt(1, sumRatios);
	for (int i = 0; i < m_emitTypeRatios.size(); i++)
	{
		rnd -= m_emitTypeRatios[i].second;

		if (rnd <= 0)
		{
			return m_emitTypeRatios[i].first;
		}
	}

	if (rnd > 0)
	{
		return "NoEmit";
	}

	return m_emitTypeRatios.back().first;
}
