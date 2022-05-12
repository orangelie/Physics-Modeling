/*
*
* < DxgiUtil.h >
* Github: https://github.com/orangelie
* // Copyright (C) 2022 by orangelie, Inc. All right reserved.
* // MIT License
*
*/





#pragma once

#pragma comment(lib, "d3d12")
#pragma comment(lib, "dxgi")
#pragma comment(lib, "d3dcompiler")

#include <d3d12.h>
#include <d3dx12.h>
#include <d3dcompiler.h>
#include <d3dcommon.h>
#include <dxgi1_4.h>

#include <DirectXMath.h>
#include <DirectXCollision.h>

using namespace DirectX;

#include "../Windows/Wrl.h"
#include "../Utility/CppStdUtil.h"

static const int gNumFramesDirty = 3;

namespace orangelie {

	namespace Utility {

		class Tools {
		public:
			static UINT CalcConstantBufferByteSize(UINT cbSize);

			static ComPtr<ID3D12Resource> CreateDefaultBuffer(
				ID3D12Device* Device,
				ID3D12GraphicsCommandList* CommandList,
				const void* data,
				UINT64 size,
				ComPtr<ID3D12Resource>& Uploader);

			static const XMFLOAT4X4 Identity();

		};
	}
}