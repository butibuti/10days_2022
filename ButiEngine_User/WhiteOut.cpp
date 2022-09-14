#include "stdafx_u.h"
#include "WhiteOut.h"

void ButiEngine::WhiteOut::OnUpdate()
{
	m_alpha = Easing::Liner(1.0f - m_vlp_timer->GetPercent());
	m_vwp_meshDrawComponent.lock()->GetCBuffer<ButiRendering::ObjectInformation>()->Get().color.w = m_alpha;

	if (m_vlp_timer->Update())
	{
		m_vlp_timer->Stop();
		gameObject.lock()->SetIsRemove(true);
	}
}

void ButiEngine::WhiteOut::OnSet()
{
}

void ButiEngine::WhiteOut::Start()
{
	m_vlp_timer = ObjectFactory::Create<RelativeTimer>(m_life);
	m_vlp_timer->Start();

	m_vwp_meshDrawComponent = gameObject.lock()->GetGameComponent<MeshDrawComponent>();
	m_alpha = 1.0f;
}

void ButiEngine::WhiteOut::OnRemove()
{
}

void ButiEngine::WhiteOut::OnShowUI()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::WhiteOut::Clone()
{
	return ObjectFactory::Create<WhiteOut>();
}
