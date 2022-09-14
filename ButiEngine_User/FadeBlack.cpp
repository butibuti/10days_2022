#include "stdafx_u.h"
#include "FadeBlack.h"

void ButiEngine::FadeBlack::OnUpdate()
{
	m_alpha = Easing::Liner(m_vlp_timer->GetPercent());
	m_vwp_meshDrawComponent.lock()->GetCBuffer<ButiRendering::ObjectInformation>()->Get().color.w = m_alpha;

	if (m_vlp_timer->Update())
	{
		m_vlp_timer->Stop();
	}
}

void ButiEngine::FadeBlack::OnSet()
{
}

void ButiEngine::FadeBlack::Start()
{
	m_vlp_timer = ObjectFactory::Create<RelativeTimer>(120);
	m_vlp_timer->Start();

	m_vwp_meshDrawComponent = gameObject.lock()->GetGameComponent<MeshDrawComponent>();
	m_alpha = 0.0f;
}

void ButiEngine::FadeBlack::OnRemove()
{
}

void ButiEngine::FadeBlack::OnShowUI()
{
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::FadeBlack::Clone()
{
	return ObjectFactory::Create<FadeBlack>();
}
