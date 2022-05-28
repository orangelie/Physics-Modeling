/*
*
* < FrameResources.h >
* Github: https://github.com/orangelie
* // Copyright (C) 2022 by orangelie, Inc. All right reserved.
* // MIT License
*
*/





#pragma once

#include "HEADER/PUBLIC/Utility/DxgiUtil.h"
#include "HEADER/PUBLIC/Gpu/UploadBuffer.h"
#include "HEADER/PUBLIC/Lighting/LightingUtils.h"
#include "HEADER/PUBLIC/Mesh/MeshTools.h"

namespace orangelie {

	struct ObjConstants {
		XMFLOAT4X4 gWorld = orangelie::Utility::Tools::Identity();
		XMFLOAT4X4 gTexTransform = orangelie::Utility::Tools::Identity();
		unsigned int gMatIndex;
	};

	struct PassConstants {
		XMFLOAT4X4 gProj = orangelie::Utility::Tools::Identity();
		XMFLOAT4X4 gView = orangelie::Utility::Tools::Identity();
		XMFLOAT4X4 gViewProj = orangelie::Utility::Tools::Identity();

		XMFLOAT4 gAmbientLight;
		XMFLOAT3 gEyePos;
		float pad1;

		orangelie::Lighting::Light gLights[16];
	};

	struct MaterialConstants {
		XMFLOAT4 gDiffuseAlbedo;
		XMFLOAT3 R0;
		float Roughness;

		XMFLOAT4X4 gMatTransform = orangelie::Utility::Tools::Identity();

		UINT SrvHeapIndex;
		UINT SelectedColor = 0;
		UINT pad2;
		UINT pad3;

		XMFLOAT4 Color;
	};

	class FrameResource {
	public:
		FrameResource(ID3D12Device* Device, UINT objCount, UINT passCount, UINT matCount, UINT textCount);
		FrameResource(const FrameResource&) = delete;
		FrameResource& operator=(const FrameResource&) = delete;
		~FrameResource();

		ComPtr<ID3D12CommandAllocator> m_CommandAllocator;
		UINT64 m_Fence;

		std::unique_ptr<orangelie::Gpu::UploadBuffer<ObjConstants>> m_ObjCB = nullptr;
		std::unique_ptr<orangelie::Gpu::UploadBuffer<PassConstants>> m_PassCB = nullptr;
		std::unique_ptr<orangelie::Gpu::UploadBuffer<MaterialConstants>> m_MatVB = nullptr;
		std::unique_ptr<orangelie::Gpu::UploadBuffer<std::vector<orangelie::Mesh::Vertex2>>> m_FontVB = nullptr;

	};
}