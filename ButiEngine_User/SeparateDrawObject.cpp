#include "stdafx_u.h"
#include "SeparateDrawObject.h"

void ButiEngine::SeparateDrawObject::OnUpdate()
{
}

void ButiEngine::SeparateDrawObject::OnSet()
{
}

void ButiEngine::SeparateDrawObject::OnRemove()
{
}

void ButiEngine::SeparateDrawObject::OnShowUI()
{
}

void ButiEngine::SeparateDrawObject::Start()
{
	//DrawObjectì¬
	std::string objectName = "DrawObject_" + gameObject.lock()->GetGameObjectTags()[0].GetID();

	m_vwp_drawObject = GetManager().lock()->AddObjectFromCereal(objectName);
	m_vwp_drawObject.lock()->SetObjectName("DrawObject_" + gameObject.lock()->GetGameObjectName());
	m_vwp_drawObject.lock()->transform->SetBaseTransform(gameObject.lock()->transform, true);

	//GameObject‚©‚çMeshDrawComponent‚Ìî•ñ‚ðŽ‚Á‚Ä‚«‚ÄDrawObject‚É’Ç‰Á
	auto meshDrawComponent = gameObject.lock()->GetGameComponent<MeshDrawComponent>();
	m_vwp_drawObject.lock()->AddGameComponent(meshDrawComponent->Clone());

	//’Ç‰ÁŒãGameObject‚É‚Â‚¢‚Ä‚¢‚éMeshDrawComponent‚Ííœ
	meshDrawComponent->AddTransform();
	meshDrawComponent->GetTransform()->SetLocalScale(0.0f);
	meshDrawComponent->SetIsRemove(true);
}

ButiEngine::Value_ptr<ButiEngine::GameComponent> ButiEngine::SeparateDrawObject::Clone()
{
	return ObjectFactory::Create<SeparateDrawObject>();
}

ButiEngine::Value_weak_ptr<ButiEngine::GameObject> ButiEngine::SeparateDrawObject::CreateDrawObject(const std::string& arg_objectName)
{
	if (m_vwp_drawObject.lock()) { return m_vwp_drawObject; }
	m_vwp_drawObject = gameObject.lock()->GetGameObjectManager().lock()->AddObjectFromCereal("DrawObject_" + arg_objectName);
	m_vwp_drawObject.lock()->transform->SetBaseTransform(gameObject.lock()->transform, true);
	return m_vwp_drawObject;
}

ButiEngine::Value_weak_ptr<ButiEngine::GameObject> ButiEngine::SeparateDrawObject::GetDrawObject()
{
	return m_vwp_drawObject;
}

void ButiEngine::SeparateDrawObject::Dead()
{
	if (m_vwp_drawObject.lock())
	{
		m_vwp_drawObject.lock()->SetIsRemove(true);
	}
}
