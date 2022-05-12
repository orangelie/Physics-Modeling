/*
*
* < InterfaceD3D12.h >
* Github: https://github.com/orangelie
* // Copyright (C) 2022 by orangelie, Inc. All right reserved.
* // MIT License
*
*/





#pragma once

#include "../Utility/DxgiUtil.h"
#include "../Windows/Windows.h"

namespace orangelie {

	namespace DxInterface {

		class InterfaceD3D12 {
		public:
			InterfaceD3D12();
			InterfaceD3D12(const InterfaceD3D12&) = delete;
			InterfaceD3D12& operator=(const InterfaceD3D12&) = delete;
			~InterfaceD3D12();

			void CreateDevice(IDXGIFactory4* DxgiFactory);
			void CreateFenceAndGetIncrementSize();
			void CreateRtvAndDsvDescriptorHeap(UINT BackBufferCount);

			ID3D12Device* GetDevice() const;
			ID3D12Fence* GetFence() const;
			ID3D12DescriptorHeap* GetRtvDescriptorHeap() const;
			ID3D12DescriptorHeap* GetDsvDescriptorHeap() const;

			UINT GetRtvDescriptorIncrementSize() const;
			UINT GetDsvDescriptorIncrementSize() const;
			UINT GetCbvSrvUavDescriptorIncrementSize() const;

		private:

		private:
			ComPtr<ID3D12Device> m_Device = nullptr;
			ComPtr<ID3D12Fence> m_Fence = nullptr;

			UINT m_RtvSize;
			UINT m_DsvSize;
			UINT m_CbvSrvUavSize;

			ComPtr<ID3D12DescriptorHeap> m_RtvDescriptorHeap = nullptr;
			ComPtr<ID3D12DescriptorHeap> m_DsvDescriptorHeap = nullptr;
		};
	}
}