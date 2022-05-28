/*
*
* < MeshTools.h >
* Github: https://github.com/orangelie
* // Copyright (C) 2022 by orangelie, Inc. All right reserved.
* // MIT License
*
*/





#pragma once

#include "../Utility/DxgiUtil.h"

namespace orangelie {

	namespace Mesh {
		struct Vertex2 {
			XMFLOAT3 Position;
			XMFLOAT3 Tangent;
			XMFLOAT3 Normal;
			XMFLOAT2 TexC;
		};

		namespace MeshColor {
			const XMFLOAT4 White = { 1.0f, 1.0f, 1.0f, 1.0f };
			const XMFLOAT4 Black = { 0.0f, 0.0f, 0.0f, 1.0f };
			const XMFLOAT4 Red = { 1.0f, 0.0f, 0.0f, 1.0f };
			const XMFLOAT4 Green = { 0.0f, 1.0f, 0.0f, 1.0f };
			const XMFLOAT4 Blue = { 0.0f, 0.0f, 1.0f, 1.0f };
			const XMFLOAT4 Yellow = { 1.0f, 1.0f, 0.0f, 1.0f };
			const XMFLOAT4 Cyan = { 0.0f, 1.0f, 1.0f, 1.0f };
			const XMFLOAT4 Magenta = { 1.0f, 0.0f, 1.0f, 1.0f };
		}

		struct SubmeshGeometry {
			UINT IndexCount;
			UINT StartIndexLocation;
			UINT BaseVertexLocation;

			BoundingBox Bounds;
		};

		struct MeshGeometry {
			std::string Name;

			ComPtr<ID3D12Resource> GpuVertexBuffer = nullptr, GpuIndexBuffer = nullptr;
			ComPtr<ID3D12Resource> GpuVertexUploader = nullptr, GpuIndexUploader = nullptr;

			ComPtr<ID3DBlob> CpuVertexBuffer = nullptr, CpuIndexBuffer = nullptr;

			UINT VertexByteStride;
			UINT VertexBufferByteSize;

			UINT IndexBufferByteSize;
			DXGI_FORMAT IndexFormat;

			std::unordered_map<std::string, SubmeshGeometry> Submeshes;

			D3D12_VERTEX_BUFFER_VIEW VertexBufferView() const {
				D3D12_VERTEX_BUFFER_VIEW vBufferView = {};
				vBufferView.BufferLocation = GpuVertexBuffer->GetGPUVirtualAddress();
				vBufferView.SizeInBytes = VertexBufferByteSize;
				vBufferView.StrideInBytes = VertexByteStride;

				return vBufferView;
			}

			D3D12_INDEX_BUFFER_VIEW IndexBufferView() const {
				D3D12_INDEX_BUFFER_VIEW iBufferView = {};
				iBufferView.BufferLocation = GpuIndexBuffer->GetGPUVirtualAddress();
				iBufferView.Format = IndexFormat;
				iBufferView.SizeInBytes = IndexBufferByteSize;

				return iBufferView;
			}
		};
	}
}