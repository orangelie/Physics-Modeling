/*
*
* < InterfaceDxgi.cpp >
* Github: https://github.com/orangelie
* // Copyright (C) 2022 by orangelie, Inc. All right reserved.
* // MIT License
*
*/





#include "HEADER/PUBLIC/DxInterface/InterfeceDxgi.h"

namespace orangelie {

	namespace DxInterface {

		InterfaceDxgi::InterfaceDxgi() {
		}

		InterfaceDxgi::~InterfaceDxgi() {
		}

		void InterfaceDxgi::BuildDxgiFactory() {
			HR(CreateDXGIFactory1(IID_PPV_ARGS(m_DxgiFactory.GetAddressOf())));
		}

		void InterfaceDxgi::CreateSwapChain(ID3D12CommandQueue* CmdQueue, HWND hWnd,
			bool isFullscreenMode, UINT BackBufferCount, DXGI_FORMAT BackBufferFormat,
			UINT ClientWidth, UINT ClientHeight) {

			DXGI_SWAP_CHAIN_DESC swapChainDesc = {};

			m_DxgiSwapChain.Reset();

			swapChainDesc.BufferCount = BackBufferCount;
			swapChainDesc.BufferDesc.Height = ClientHeight;
			swapChainDesc.BufferDesc.Width = ClientWidth;
			swapChainDesc.BufferDesc.Format = BackBufferFormat;
			swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
			swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
			swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
			swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
			swapChainDesc.OutputWindow = hWnd;
			swapChainDesc.SampleDesc.Count = 1;
			swapChainDesc.SampleDesc.Quality = 0;
			swapChainDesc.Windowed = (!isFullscreenMode);
			swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

			HR(m_DxgiFactory->CreateSwapChain(CmdQueue, &swapChainDesc, m_DxgiSwapChain.GetAddressOf()));
		}

		IDXGIFactory4* InterfaceDxgi::GetFactory4() const {
			return m_DxgiFactory.Get();
		}

		ComPtr<IDXGISwapChain> InterfaceDxgi::GetSwapChain() const {
			return m_DxgiSwapChain;
		}
	}
}