/*
*
* < InterfaceD3D12.cpp >
* Github: https://github.com/orangelie
* // Copyright (C) 2022 by orangelie, Inc. All right reserved.
* // MIT License
*
*/





#include "HEADER/PUBLIC/DxInterface/InterfaceD3D12.h"

namespace orangelie {

	namespace DxInterface {

		InterfaceD3D12::InterfaceD3D12() {
		}

		InterfaceD3D12::~InterfaceD3D12() {
		}

		void InterfaceD3D12::CreateDevice(IDXGIFactory4* DxgiFactory) {
			LRESULT lResult =  D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(m_Device.GetAddressOf()));

			// Software Adapter
			if (FAILED(lResult)) {
				ComPtr<IDXGIAdapter> warpAdapter;
				HR(DxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(warpAdapter.GetAddressOf())));
				D3D12CreateDevice(warpAdapter.Get(), D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(m_Device.GetAddressOf()));
			}
		}

		void InterfaceD3D12::CreateFenceAndGetIncrementSize() {
			HR(m_Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(m_Fence.GetAddressOf())));

			m_RtvSize = m_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
			m_DsvSize = m_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
			m_CbvSrvUavSize = m_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
		}

		void InterfaceD3D12::CreateRtvAndDsvDescriptorHeap(UINT BackBufferCount) {
			D3D12_DESCRIPTOR_HEAP_DESC RTVResourceDesc = {};

			RTVResourceDesc.NodeMask = 0;
			RTVResourceDesc.NumDescriptors = BackBufferCount;
			RTVResourceDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			RTVResourceDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			
			D3D12_DESCRIPTOR_HEAP_DESC DSVResourceDesc = {};

			DSVResourceDesc.NodeMask = 0;
			DSVResourceDesc.NumDescriptors = 1;
			DSVResourceDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			DSVResourceDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;

			HR(m_Device->CreateDescriptorHeap(&RTVResourceDesc, IID_PPV_ARGS(m_RtvDescriptorHeap.GetAddressOf())));
			HR(m_Device->CreateDescriptorHeap(&DSVResourceDesc, IID_PPV_ARGS(m_DsvDescriptorHeap.GetAddressOf())));
		}

		ID3D12Device* InterfaceD3D12::GetDevice() const {
			return m_Device.Get();
		}

		ID3D12Fence* InterfaceD3D12::GetFence() const {
			return m_Fence.Get();
		}

		ID3D12DescriptorHeap* InterfaceD3D12::GetRtvDescriptorHeap() const {
			return m_RtvDescriptorHeap.Get();
		}

		ID3D12DescriptorHeap* InterfaceD3D12::GetDsvDescriptorHeap() const {
			return m_DsvDescriptorHeap.Get();
		}

		UINT InterfaceD3D12::GetRtvDescriptorIncrementSize() const {
			return m_RtvSize;
		}

		UINT InterfaceD3D12::GetDsvDescriptorIncrementSize() const {
			return m_DsvSize;
		}

		UINT InterfaceD3D12::GetCbvSrvUavDescriptorIncrementSize() const {
			return m_CbvSrvUavSize;
		}
	}
}