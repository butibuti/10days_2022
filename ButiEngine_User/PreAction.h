#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {
	
	enum class GunActionType
	{
		AssaultRifle,
		GrenadeLauncher,
		Shotgun,
		LastAttack,
	};

	enum class PreActioinPhase
	{
		LookCamera,
		Wait,
		Return,
	};

	class PreAction :public GameComponent
	{
	public:
		PreAction(){}
		PreAction(const GunActionType arg_gunAction)
			: m_gunActionType(arg_gunAction)
		{
		}
		std::string GetGameComponentName()const override {
			return "PreAction";
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

		void SetGunActionType(const GunActionType arg_gunActionType) { m_gunActionType = arg_gunActionType; }
	private:
		void StartLookCameraPhase();
		void UpdateLookCameraPhase();
		void StartWaitPhase();
		void UpdateWaitPhase();
		void StartReturnPhase();
		void UpdateReturnPhase();

		void ChangeGun();
		void AddGunAction();
		void AddStartTransformAnimation_Player();
		void AddFinishTransformAnimation_Player();
		void AddStartTransformAnimation_RightGun();
		void AddFinishTransformAnimation_RightGun();
		void AddStartTransformAnimation_LeftGun();
		void AddFinishTransformAnimation_LeftGun();

		PreActioinPhase m_phase;

		Value_ptr<RelativeTimer> m_vlp_lookCameraTimer;
		Value_ptr<RelativeTimer> m_vlp_waitTimer;
		Value_ptr<RelativeTimer> m_vlp_returnTimer;


		GunActionType m_gunActionType;

		Value_weak_ptr<GameObject> m_vwp_drawObject;
		Value_weak_ptr<GameObject> m_vwp_rightGun;
		Value_weak_ptr<GameObject> m_vwp_leftGun;

		Value_ptr<Transform> m_vlp_startPlayerTransform;
		Value_ptr<Transform> m_vlp_startRightGunTransform;
		Value_ptr<Transform> m_vlp_startLeftGunTransform;
	};

}

BUTI_REGIST_GAMECOMPONENT(PreAction, true);