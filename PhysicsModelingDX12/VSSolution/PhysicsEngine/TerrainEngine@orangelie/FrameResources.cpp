/*
*
* < FrameResource.cpp >
* Github: https://github.com/orangelie
* // Copyright (C) 2022 by orangelie, Inc. All right reserved.
* // MIT License
*
*/





#include "FrameResources.h"

namespace orangelie {

	FrameResource::FrameResource(ID3D12Device* Device, UINT objCount, UINT passCount, UINT matCount, UINT textCount) {
		HR(Device->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			IID_PPV_ARGS(m_CommandAllocator.GetAddressOf())));
		m_Fence = 0;

		m_ObjCB = std::make_unique<orangelie::Gpu::UploadBuffer<ObjConstants>>(Device, objCount, true);
		m_PassCB = std::make_unique<orangelie::Gpu::UploadBuffer<PassConstants>>(Device, passCount, true);
		m_MatVB = std::make_unique<orangelie::Gpu::UploadBuffer<MaterialConstants>>(Device, matCount, false);
		m_FontVB = std::make_unique<orangelie::Gpu::UploadBuffer<std::vector<orangelie::Mesh::Vertex2>>>(Device, textCount, false);
	}

	FrameResource::~FrameResource() {
	}
}