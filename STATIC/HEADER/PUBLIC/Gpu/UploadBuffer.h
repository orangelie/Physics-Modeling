/*
*
* < UploadBuffer.h >
* Github: https://github.com/orangelie
* // Copyright (C) 2022 by orangelie, Inc. All right reserved.
* // MIT License
*
*/





#pragma once

#include "../Utility/DxgiUtil.h"

namespace orangelie {

	namespace Gpu {

		template <class _Tp>
		class UploadBuffer {
		public:
			UploadBuffer() = delete;
			UploadBuffer(const UploadBuffer&) = delete;
			UploadBuffer& operator=(const UploadBuffer&) = delete;
			UploadBuffer(ID3D12Device* Device, size_t elementCount, bool isConstantBuffer) {
				m_ElementByteSize = sizeof(_Tp);
				if (isConstantBuffer) m_ElementByteSize = orangelie::Utility::Tools::CalcConstantBufferByteSize(sizeof(_Tp));

				using orangelie::CppStdUtil::unmove;
				HR(Device->CreateCommittedResource(&unmove(CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD)),
					D3D12_HEAP_FLAG_NONE, &unmove(CD3DX12_RESOURCE_DESC::Buffer(elementCount * m_ElementByteSize)), D3D12_RESOURCE_STATE_GENERIC_READ,
					nullptr, IID_PPV_ARGS(m_MappedResource.GetAddressOf())));

				HR(m_MappedResource->Map(0, nullptr, (void**)&MappedData));
			}

			~UploadBuffer() {
				if (m_MappedResource != nullptr) {
					m_MappedResource->Unmap(0, nullptr);
				}

				m_MappedResource = nullptr;
			}

			ID3D12Resource* Resource() const {
				return m_MappedResource.Get();
			}

			void CopyData(UINT elementIndex, const _Tp& data) {
				CopyMemory(&MappedData[elementIndex * m_ElementByteSize], &data, sizeof(_Tp));
			}

		private:
			ComPtr<ID3D12Resource> m_MappedResource = nullptr;
			BYTE* MappedData = nullptr;
			size_t m_ElementByteSize;

		};
	}
}