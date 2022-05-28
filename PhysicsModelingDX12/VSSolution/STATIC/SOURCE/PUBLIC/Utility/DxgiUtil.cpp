/*
*
* < DxgiUtil.cpp >
* Github: https://github.com/orangelie
* // Copyright (C) 2022 by orangelie, Inc. All right reserved.
* // MIT License
*
*/





#include "HEADER/PUBLIC/Utility/DxgiUtil.h"

namespace orangelie {

	namespace Utility {

		
		UINT Tools::CalcConstantBufferByteSize(UINT cbSize) {
			return (cbSize + 255) & ~255;
		}

		ComPtr<ID3D12Resource> Tools::CreateDefaultBuffer(
			ID3D12Device* Device,
			ID3D12GraphicsCommandList* CommandList,
			const void* data,
			UINT64 size,
			ComPtr<ID3D12Resource>& Uploader) {
			using orangelie::CppStdUtil::unmove;

			ComPtr<ID3D12Resource> defaultBuffer;

			HR(Device->CreateCommittedResource(
				&unmove(CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT)),
				D3D12_HEAP_FLAG_NONE,
				&unmove(CD3DX12_RESOURCE_DESC::Buffer(size)),
				D3D12_RESOURCE_STATE_COMMON,
				nullptr,
				IID_PPV_ARGS(defaultBuffer.GetAddressOf())));

			HR(Device->CreateCommittedResource(
				&unmove(CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD)),
				D3D12_HEAP_FLAG_NONE,
				&unmove(CD3DX12_RESOURCE_DESC::Buffer(size)),
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(Uploader.GetAddressOf())));

			CommandList->ResourceBarrier(1,
				&unmove(CD3DX12_RESOURCE_BARRIER::Transition(
					defaultBuffer.Get(),
					D3D12_RESOURCE_STATE_COMMON,
					D3D12_RESOURCE_STATE_COPY_DEST)));

			D3D12_SUBRESOURCE_DATA subResourceData = {};

			subResourceData.pData = data;
			subResourceData.RowPitch = size;
			subResourceData.SlicePitch = subResourceData.RowPitch;

			UpdateSubresources<1>(CommandList, defaultBuffer.Get(), Uploader.Get(), 0, 0, 1, &subResourceData);

			CommandList->ResourceBarrier(1,
				&unmove(CD3DX12_RESOURCE_BARRIER::Transition(
					defaultBuffer.Get(),
					D3D12_RESOURCE_STATE_COPY_DEST,
					D3D12_RESOURCE_STATE_GENERIC_READ)));

			return defaultBuffer;
		}

		const XMFLOAT4X4 Tools::Identity() {
			const XMFLOAT4X4 i = XMFLOAT4X4(
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f);

			return i;
		}
	}
}