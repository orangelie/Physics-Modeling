/*
*
* < FrameResources.h >
* Github: https://github.com/orangelie
* // Copyright (C) 2022 by orangelie, Inc. All right reserved.
* // MIT License
*
*/





#pragma once

#include "../Utility/DxgiUtil.h"

namespace orangelie {

	namespace Camera {

		class DefaultCamera {
		public:
			DefaultCamera();
			DefaultCamera(const DefaultCamera&) = delete;
			DefaultCamera& operator=(const DefaultCamera&) = delete;
			~DefaultCamera();

			void SetLens(float width, float height, float fovY, float aspectRatio, float nearZ, float farZ);
			void UpdateViewMatrix();

			void LookAt(XMFLOAT3 pos, XMFLOAT3 target, XMFLOAT3 up);
			void LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR up);


			// Data
			XMFLOAT3 GetPosition() const;
			XMFLOAT4X4 GetViewMatrix() const;
			XMFLOAT4X4 GetProjectionMatrix() const;
			XMFLOAT4X4 GetOrthoMatrix() const;

			float GetNearZ() const;
			float GetFarZ() const;

			// Movement
			void SetPosition(float x, float y, float z);

			void Strafe(float d);
			void Walk(float d);

			void Pitch(float d);
			void RotateY(float d);

		private:
			XMFLOAT3 m_Position = { 0.0f, 0.0f, 0.0f };
			XMFLOAT3 m_Look = { 0.0f, 0.0f, 1.0f };
			XMFLOAT3 m_Up = { 0.0f, 1.0f, 0.0f };
			XMFLOAT3 m_Right = { 1.0f, 0.0f, 0.0f };

			XMFLOAT4X4 m_View = orangelie::Utility::Tools::Identity();
			XMFLOAT4X4 m_Projection = orangelie::Utility::Tools::Identity();
			XMFLOAT4X4 m_Ortho = orangelie::Utility::Tools::Identity();

			float m_NearWindowHeight, m_FarWindowHeight;

			float m_FovY, m_Aspect;
			float m_NearZ, m_FarZ;
			bool m_ViewDirty = false;

		};
	}
}