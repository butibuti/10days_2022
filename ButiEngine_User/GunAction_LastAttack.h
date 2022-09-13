#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Player;
	class Gun;

	enum class GunAction_LastAttackPhase
	{
		AR_FirstMove,
		AR_FirstShoot,
		AR_SecondMove,
		AR_SecondShoot,
		AR_ThirdMove,
		AR_ThirdShoot,
		AR_SG_Wait,
		SG_Shoot,
		SG_GL_Wait,
		GL_MoveOffScreen,
		GL_MoveOffScreenWait,
		GL_FirstShoot,
		GL_SecondShoot,
		GL_ShootWait,
		GL_ReturnCenter,
	};

	class GunAction_LastAttack :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "GunAction_LastAttack";
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
		void StartAR_FirstShootPhase();
		void UpdateAR_FirstShootPhase();

		void StartAR_FirstMovePhase();
		void UpdateAR_FirstMovePhase();

		void StartAR_SecondShootPhase();
		void UpdateAR_SecondShootPhase();

		void StartAR_SecondMovePhase();
		void UpdateAR_SecondMovePhase();

		void StartAR_ThirdMovePhase();
		void UpdateAR_ThirdMovePhase();

		void StartAR_ThirdShootPhase();
		void UpdateAR_ThirdShootPhase();

		void StartAR_SG_WaitPhase();
		void UpdateAR_SG_WaitPhase();

		void StartSG_ShootPhase();
		void UpdateSG_ShootPhase();

		void StartSG_GLWaitPhase();
		void UpdateSG_GLWaitPhase();

		void StartGL_MoveOffScreenPhase();
		void UpdateGL_MoveOffScreenPhase();

		void StartGL_MoveOffScreenWaitPhase();
		void UpdateGL_MoveOffScreenWaitPhase();

		void StartGL_FirstShootPhase();
		void UpdateGL_FirstShootPhase();

		void StartGL_SecondShootPhase();
		void UpdateGL_SecondShootPhase();

		void StartGL_ShootWaitPhase();
		void UpdateGL_ShootWaitPhase();

		void StartGL_ReturnCenterPhase();
		void UpdateGL_ReturnCenterPhase();

		void AddPositionAnimation(const Vector3& arg_targetPos, const float arg_speed, const Easing::EasingType arg_easeType);

		GunAction_LastAttackPhase m_phase;

		Value_ptr<RelativeTimer> m_vlp_AR_firstMovePhaseTimer;

		Value_ptr<RelativeTimer> m_vlp_AR_firstShootPhaseTimer;

		Value_ptr<RelativeTimer> m_vlp_AR_secondMovePhaseTimer;

		Value_ptr<RelativeTimer> m_vlp_AR_secondShootPhaseTimer;

		Value_ptr<RelativeTimer> m_vlp_AR_thirdMovePhaseTimer;

		Value_ptr<RelativeTimer> m_vlp_AR_thirdShootPhaseTimer;

		Value_ptr<RelativeTimer> m_vlp_AR_SG_waitPhaseTimer;

		Value_ptr<RelativeTimer> m_vlp_SG_shootPhaseTimer;
		Value_ptr<RelativeTimer> m_vlp_moveShootTimer;
		std::int32_t m_maxShootCount;
		std::int32_t m_shootCount;
		float m_radius;

		Value_ptr<RelativeTimer> m_vlp_SG_GL_waitPhaseTimer;

		Value_ptr<RelativeTimer> m_vlp_GL_moveOffScreenPhaseTimer;

		Value_ptr<RelativeTimer> m_vlp_GL_moveOffScreenWaitPhaseTimer;

		Value_ptr<RelativeTimer> m_vlp_GL_firstShootPhaseTimer;

		Value_ptr<RelativeTimer> m_vlp_GL_secondShootPhaseTimer;

		Value_ptr<RelativeTimer> m_vlp_GL_shootWaitPhaseTimer;

		Value_ptr<RelativeTimer> m_vlp_GL_returnCenterPhaseTimer;


		Value_ptr<Transform> m_vwp_startTransform;
		Value_weak_ptr<GameObject> m_vwp_boss;

		Value_weak_ptr<Gun> m_vwp_rightGunComponent;
		Value_weak_ptr<Gun> m_vwp_leftGunComponent;
		Value_weak_ptr<GameObject> m_vwp_drawObject;
	};

}

BUTI_REGIST_GAMECOMPONENT(GunAction_LastAttack, true);