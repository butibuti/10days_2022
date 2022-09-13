#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {
	class Camera :public GameComponent
	{
	public:
		std::string GetGameComponentName()const override {
			return "Camera";
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

		void StartChasePlayer();
		void StopChasePlayer();
		void Approach(const std::int32_t arg_frame);
		void ReturnDefault(const std::int32_t arg_frame);
		void StartShake(const std::int32_t arg_shakeFrame, const Vector3& arg_amplitude, const std::int32_t arg_shakeIntervalFrame);
	private:
		Value_weak_ptr<GameObject> m_vwp_parent;
		Value_ptr<Transform> m_vlp_defaultTransform;
		Value_ptr<Transform> m_vlp_parentDefaultTransform;
	};

}

BUTI_REGIST_GAMECOMPONENT(Camera, true);