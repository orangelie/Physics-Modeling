/*
*
* < InterfaceDxgi.h >
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

		class InterfaceDxgi {
		public:
			InterfaceDxgi();
			InterfaceDxgi(const InterfaceDxgi&) = delete;
			InterfaceDxgi& operator=(const InterfaceDxgi&) = delete;
			~InterfaceDxgi();

			void BuildDxgiFactory();
			void CreateSwapChain(ID3D12CommandQueue* CmdQueue, HWND hWnd,
				bool isFullscreenMode, UINT BackBufferCount, DXGI_FORMAT BackBufferFormat,
				UINT ClientWidth, UINT ClientHeight);

			IDXGIFactory4* GetFactory4() const;
			ComPtr<IDXGISwapChain> GetSwapChain() const;

		private:

		private:
			ComPtr<IDXGIFactory4> m_DxgiFactory = nullptr;
			ComPtr<IDXGISwapChain> m_DxgiSwapChain = nullptr;

		};
	}
}