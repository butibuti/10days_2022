#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Player;
	class Gun;

	class GunAction_Gutling :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "GunAction_Gutling";
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
	private:
		Value_ptr<RelativeTimer> m_vlp_rotateTimer;

		Value_weak_ptr<Player> m_vwp_playerComponent;
		Value_weak_ptr<Gun> m_vwp_gunComponent;
		Value_weak_ptr<GameObject> m_vwp_drawObject;
		Value_weak_ptr<Transform> m_vwp_lookTarget;
	};

}

BUTI_REGIST_GAMECOMPONENT(GunAction_Gutling, true);