/*
*
* < FrameResources.h >
* Github: https://github.com/orangelie
* // Copyright (C) 2022 by orangelie, Inc. All right reserved.
* // MIT License
*
*/





#include "HEADER/PUBLIC/Camera/DefaultCamera.h"

namespace orangelie {

	namespace Camera {

		DefaultCamera::DefaultCamera() {
			SetLens(920, 860, 0.25f * XM_PI, 1.0f, 1.0f, 1000.0f);
		}

		DefaultCamera::~DefaultCamera() {
		}

		void DefaultCamera::SetLens(float width, float height, float fovY, float aspectRatio, float nearZ, float farZ) {
			m_Aspect = aspectRatio;
			m_FovY = fovY;
			m_NearZ = nearZ;
			m_FarZ = farZ;

			m_NearWindowHeight = m_NearZ * 2.0f * tanf(0.5f * fovY);
			m_FarWindowHeight = m_FarZ * 2.0f * tanf(0.5f * fovY);

			auto Proj = XMMatrixPerspectiveFovLH(fovY, aspectRatio, nearZ, farZ);
			XMStoreFloat4x4(&m_Projection, Proj);

			auto Orth = XMMatrixOrthographicLH(width, height, nearZ, farZ);
			XMStoreFloat4x4(&m_Ortho, Orth);
		}

		void DefaultCamera::UpdateViewMatrix() {
			if (m_ViewDirty) {
				XMVECTOR P = XMLoadFloat3(&m_Position);
				XMVECTOR L = XMLoadFloat3(&m_Look);
				XMVECTOR U = XMLoadFloat3(&m_Up);
				XMVECTOR R = XMLoadFloat3(&m_Right);

				L = XMVector3Normalize(L);
				U = XMVector3Normalize(XMVector3Cross(L, R));
				R = XMVector3Cross(U, L);

				XMStoreFloat3(&m_Look, L);
				XMStoreFloat3(&m_Up, U);
				XMStoreFloat3(&m_Right, R);

				float x = -XMVectorGetX(XMVector3Dot(P, R));
				float y = -XMVectorGetX(XMVector3Dot(P, U));
				float z = -XMVectorGetX(XMVector3Dot(P, L));

				m_View(0, 0) = m_Right.x; m_View(0, 1) = m_Up.x; m_View(0, 2) = m_Look.x; m_View(0, 3) = 0.0f;
				m_View(1, 0) = m_Right.y; m_View(1, 1) = m_Up.y; m_View(1, 2) = m_Look.y; m_View(1, 3) = 0.0f;
				m_View(2, 0) = m_Right.z; m_View(2, 1) = m_Up.z; m_View(2, 2) = m_Look.z; m_View(2, 3) = 0.0f;
				m_View(3, 0) = x; m_View(3, 1) = y; m_View(3, 2) = z; m_View(3, 3) = 1.0f;

				m_ViewDirty = false;
			}
		}

		void DefaultCamera::LookAt(XMFLOAT3 pos, XMFLOAT3 target, XMFLOAT3 up) {
			XMVECTOR Position = XMLoadFloat3(&pos);
			XMVECTOR Target = XMLoadFloat3(&target);
			XMVECTOR Up = XMLoadFloat3(&up);

			LookAt(Position, Target, Up);

			m_ViewDirty = true;
		}

		void DefaultCamera::LookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR up) {
			XMVECTOR look = XMVector3Normalize(XMVectorSubtract(target, pos));
			XMStoreFloat3(&m_Look, look);

			XMVECTOR right = XMVector3Normalize(XMVector3Cross(up, look));
			XMStoreFloat3(&m_Right, right);

			XMStoreFloat3(&m_Up, XMVector3Cross(look, right));

			XMStoreFloat3(&m_Position, pos);

			m_ViewDirty = true;
		}

		XMFLOAT3 DefaultCamera::GetPosition() const {
			return m_Position;
		}

		XMFLOAT4X4 DefaultCamera::GetViewMatrix() const {
			return m_View;
		}

		XMFLOAT4X4 DefaultCamera::GetProjectionMatrix() const {
			return m_Projection;
		}

		XMFLOAT4X4 DefaultCamera::GetOrthoMatrix() const {
			return m_Ortho;
		}

		float DefaultCamera::GetNearZ() const {
			return m_NearZ;
		}

		float DefaultCamera::GetFarZ() const {
			return m_FarZ;
		}

		void DefaultCamera::SetPosition(float x, float y, float z) {
			m_Position = { x, y, z };
			m_ViewDirty = true;
		}

		void DefaultCamera::Strafe(float d) {
			XMVECTOR D = XMVectorReplicate(d);
			XMVECTOR R = XMLoadFloat3(&m_Right);
			XMVECTOR P = XMLoadFloat3(&m_Position);

			XMStoreFloat3(&m_Position, XMVectorMultiplyAdd(D, R, P));

			m_ViewDirty = true;
		}

		void DefaultCamera::Walk(float d) {
			XMVECTOR D = XMVectorReplicate(d);
			XMVECTOR L = XMLoadFloat3(&m_Look);
			XMVECTOR P = XMLoadFloat3(&m_Position);

			XMStoreFloat3(&m_Position, XMVectorMultiplyAdd(D, L, P));

			m_ViewDirty = true;
		}

		void DefaultCamera::Pitch(float d, float dt) {
			XMVECTOR R = XMLoadFloat3(&m_Right);
			XMVECTOR U = XMLoadFloat3(&m_Up);
			XMVECTOR L = XMLoadFloat3(&m_Look);

			XMMATRIX rot = XMMatrixRotationAxis(R, d);

			XMStoreFloat3(&m_Up, XMVector3TransformNormal(U, rot));
			XMStoreFloat3(&m_Look, XMVector3TransformNormal(L, rot));

			m_ViewDirty = true;
		}

		void DefaultCamera::RotateY(float d, float dt) {
			XMVECTOR R = XMLoadFloat3(&m_Right);
			XMVECTOR U = XMLoadFloat3(&m_Up);
			XMVECTOR L = XMLoadFloat3(&m_Look);

			XMMATRIX rot = XMMatrixRotationY(d);

			XMStoreFloat3(&m_Up, XMVector3TransformNormal(U, rot));
			XMStoreFloat3(&m_Right, XMVector3TransformNormal(R, rot));
			XMStoreFloat3(&m_Look, XMVector3TransformNormal(L, rot));

			m_ViewDirty = true;
		}

	}
}