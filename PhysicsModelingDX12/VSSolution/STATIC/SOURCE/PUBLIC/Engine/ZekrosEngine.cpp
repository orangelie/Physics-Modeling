/*
*
* < ZekrosEngine.cpp >
* Github: https://github.com/orangelie
* // Copyright (C) 2022 by orangelie, Inc. All right reserved.
* // MIT License
*
*/





#include "HEADER/PUBLIC/Engine/ZekrosEngine.h"

namespace orangelie {

	namespace Engine {

		ZekrosEngine::ZekrosEngine() {
			gZekrosEngine = this;
		}

		ZekrosEngine::~ZekrosEngine() {
			if (m_Device != nullptr)
				FlushCommandQueue();
		}

		void ZekrosEngine::Initialize(int maxScreenWidth, int maxScreenHeight, bool isFullscreenMode) {
#if defined(DEBUG) || defined(_DEBUG)
			ComPtr<ID3D12Debug> DebugLayer;
			HR(D3D12GetDebugInterface(IID_PPV_ARGS(DebugLayer.GetAddressOf())));
			DebugLayer->EnableDebugLayer();
#endif


			// Win32
			m_Win32 = std::make_unique<orangelie::Windows::Win32>();
			m_Win32->Intialize(WindowProcedure, maxScreenWidth, maxScreenHeight, isFullscreenMode);
			m_hWnd = m_Win32->GetHwnd();

			// Factory
			m_DxgiInterface = std::make_unique<orangelie::DxInterface::InterfaceDxgi>();
			m_DxgiInterface->BuildDxgiFactory();

			// DirectX 12 Device
			m_D3D12Interface = std::make_unique<orangelie::DxInterface::InterfaceD3D12>();
			m_D3D12Interface->CreateDevice(m_DxgiInterface->GetFactory4());
			m_Device = m_D3D12Interface->GetDevice();

			// Resource Fence
			m_D3D12Interface->CreateFenceAndGetIncrementSize();
			m_Fence = m_D3D12Interface->GetFence();
			m_RtvSize = m_D3D12Interface->GetRtvDescriptorIncrementSize();
			m_DsvSize = m_D3D12Interface->GetDsvDescriptorIncrementSize();
			m_CbvSrvUavSize = m_D3D12Interface->GetCbvSrvUavDescriptorIncrementSize();

			// Command Objects
			CreateCommandObjects();

			// SwapChain
			m_DxgiInterface->CreateSwapChain(m_CommandQueue.Get(),
				m_Win32->GetHwnd(), isFullscreenMode, gBackBufferCount, m_BackBufferFormat,
				m_ClientWidth, m_ClientHeight);

			// RenderTagetView Heap & DepthStencilView Heap
			m_D3D12Interface->CreateRtvAndDsvDescriptorHeap(gBackBufferCount);

			OnResize();

			init();
		}

		void ZekrosEngine::Run() {
			MSG msg = {};

			m_GameTimer.Reset();

			while(msg.message != WM_QUIT) {
				if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
					TranslateMessage(&msg);
					DispatchMessageW(&msg);
				}
				else {
					m_GameTimer.Tick();

					if (m_IsEnginePaused == false) {
						update(m_GameTimer.DeltaTime());
						draw(m_GameTimer.DeltaTime());
					}
					else {
						Sleep(100);
					}
				}
			}
		}
		
		LRESULT ZekrosEngine::MessageHandler(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam) {
			switch (uMessage) {
			case WM_ACTIVATE:
				if (LOWORD(wParam) == WA_INACTIVE) {
					m_GameTimer.Stop();
					m_IsEnginePaused = true;
				}
				else {
					m_GameTimer.Start();
					m_IsEnginePaused = false;
				}
				return 0;

			case WM_SIZE:
				m_ClientWidth = LOWORD(lParam);
				m_ClientHeight = HIWORD(lParam);

				if (wParam == SIZE_MINIMIZED) {
					m_IsSizeMinimized = true;
					m_IsSizeMaximized = false;
					m_IsEnginePaused = true;
				}

				else if (wParam == SIZE_MAXIMIZED) {
					m_IsSizeMinimized = false;
					m_IsSizeMaximized = true;
					m_IsEnginePaused = false;
					OnResize();
				}

				else if (wParam == SIZE_RESTORED) {
					if (m_IsSizeMinimized) {
						m_IsSizeMinimized = false;
						m_IsEnginePaused = false;
						OnResize();
					}
					else if (m_IsSizeMaximized) {
						m_IsSizeMaximized = false;
						m_IsEnginePaused = false;
						OnResize();
					}
					else if (m_IsResizing) {

					}
				}
				
				return 0;

			case WM_ENTERSIZEMOVE:
				m_IsEnginePaused = true;
				m_IsResizing = true;
				m_GameTimer.Stop();
				return 0;
			case WM_EXITSIZEMOVE:
				m_IsEnginePaused = false;
				m_IsResizing = false;
				m_GameTimer.Start();
				OnResize();
				return 0;

			case WM_MOUSEMOVE:
				MouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), m_GameTimer.DeltaTime());
				return 0;

			case WM_RBUTTONDOWN:
			case WM_MBUTTONDOWN:
			case WM_LBUTTONDOWN:
				MouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
				return 0;

			case WM_RBUTTONUP:
			case WM_MBUTTONUP:
			case WM_LBUTTONUP:
				MouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
				return 0;

			case WM_GETMINMAXINFO:
				((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
				((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;
				return 0;

			case WM_MENUCHAR:
				return MAKELRESULT(0, MNC_CLOSE);

			case WM_KEYDOWN:
				if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
					PostQuitMessage(0);
				}
				return 0;

			case WM_DESTROY:
			case WM_CLOSE:
				PostQuitMessage(0);
				return 0;
			}

			return DefWindowProcW(hWnd, uMessage, wParam, lParam);
		}

		void ZekrosEngine::OnResize() {
			// assert(m_D3D12Interface->GetDevice());
			// assert(m_DxgiInterface->GetSwapChain());
			// assert(m_CommandAllocator);

			FlushCommandQueue();

			HR(m_CommandList->Reset(m_CommandAllocator.Get(), nullptr));
			for (int i = 0; i < gBackBufferCount; ++i) {
				m_SwapChainBuffer[i].Reset();
			}
			m_DepthStencilBuffer.Reset();

			HR(m_DxgiInterface->GetSwapChain()->ResizeBuffers((UINT)gBackBufferCount,
				m_ClientWidth, m_ClientHeight, m_BackBufferFormat, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));

			m_CurrentSwapBufferIndex = 0;

			CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_D3D12Interface->GetRtvDescriptorHeap()->GetCPUDescriptorHandleForHeapStart());
			auto rtvOffset = m_D3D12Interface->GetDsvDescriptorIncrementSize();
			for (int i = 0; i < gBackBufferCount; ++i) {
				HR(m_DxgiInterface->GetSwapChain()->GetBuffer(i, IID_PPV_ARGS(m_SwapChainBuffer[i].GetAddressOf())));
				m_Device->CreateRenderTargetView(m_SwapChainBuffer[i].Get(), nullptr, rtvHandle);
				rtvHandle.Offset(1, rtvOffset);
			}

			D3D12_RESOURCE_DESC dsvResourceDesc = {};
			dsvResourceDesc.Alignment = 0;
			dsvResourceDesc.DepthOrArraySize = 1;
			dsvResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
			dsvResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
			dsvResourceDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
			dsvResourceDesc.Width = m_ClientWidth;
			dsvResourceDesc.Height = m_ClientHeight;
			dsvResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
			dsvResourceDesc.MipLevels = 1;
			dsvResourceDesc.SampleDesc.Count = 1;
			dsvResourceDesc.SampleDesc.Quality = 0;

			D3D12_CLEAR_VALUE clearValue = {};
			clearValue.Format = m_DepthStencilFormat;
			clearValue.DepthStencil.Depth = 1.0f;
			clearValue.DepthStencil.Stencil = 0;

			using orangelie::CppStdUtil::unmove;
			HR(m_Device->CreateCommittedResource(
				&unmove(CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT)),
				D3D12_HEAP_FLAG_NONE,
				&dsvResourceDesc,
				D3D12_RESOURCE_STATE_COMMON,
				&clearValue,
				IID_PPV_ARGS(m_DepthStencilBuffer.GetAddressOf())));

			D3D12_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
			depthStencilViewDesc.Flags = D3D12_DSV_FLAG_NONE;
			depthStencilViewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
			depthStencilViewDesc.Texture2D.MipSlice = 0;
			depthStencilViewDesc.Format = m_DepthStencilFormat;

			m_Device->CreateDepthStencilView(m_DepthStencilBuffer.Get(),
				&depthStencilViewDesc,
				m_D3D12Interface->GetDsvDescriptorHeap()->GetCPUDescriptorHandleForHeapStart());

			m_CommandList->ResourceBarrier(1,
				&unmove(CD3DX12_RESOURCE_BARRIER::Transition(
					m_DepthStencilBuffer.Get(),
					D3D12_RESOURCE_STATE_COMMON,
					D3D12_RESOURCE_STATE_DEPTH_WRITE)));

			SubmitCommandList();
			FlushCommandQueue();

			m_Viewport.Width = static_cast<float>(m_ClientWidth);
			m_Viewport.Height = static_cast<float>(m_ClientHeight);
			m_Viewport.MinDepth = 0.0f;
			m_Viewport.MaxDepth = 1.0f;
			m_Viewport.TopLeftX = 0;
			m_Viewport.TopLeftY = 0;

			m_ScissorRect = { 0, 0, m_ClientWidth, m_ClientHeight };
		}

		void ZekrosEngine::FlushCommandQueue() {
			auto fence = m_D3D12Interface->GetFence();

			++m_CurrentFenceCount;
			HR(m_CommandQueue->Signal(fence, m_CurrentFenceCount));

			if (fence->GetCompletedValue() < m_CurrentFenceCount) {
				HANDLE hEvent = CreateEventEx(
					0,
					0,
					0,
					EVENT_ALL_ACCESS);

				HR(fence->SetEventOnCompletion(
					m_CurrentFenceCount,
					hEvent));

				WaitForSingleObject(hEvent, 0xFFFFFFFF);
				CloseHandle(hEvent);
			}
		}

		void ZekrosEngine::PresentSwapChain() {
			m_DxgiInterface->GetSwapChain()->Present(0, 0);
		}

		void ZekrosEngine::SubmitCommandList() {
			HR(m_CommandList->Close());
			ID3D12CommandList* cmdLists[] = { m_CommandList.Get() };
			m_CommandQueue->ExecuteCommandLists(_countof(cmdLists), cmdLists);
		}

		void ZekrosEngine::CreateCommandObjects() {
			HR(m_Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
				IID_PPV_ARGS(m_CommandAllocator.GetAddressOf())));

			HR(m_Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
				m_CommandAllocator.Get(),
				nullptr,
				IID_PPV_ARGS(m_CommandList.GetAddressOf())));


			D3D12_COMMAND_QUEUE_DESC CmdQueueDesc = {};

			CmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
			CmdQueueDesc.Priority = 0;
			CmdQueueDesc.NodeMask = 0;
			CmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

			HR(m_Device->CreateCommandQueue(&CmdQueueDesc,
				IID_PPV_ARGS(m_CommandQueue.GetAddressOf())));

			m_CommandList->Close();
		}

		std::array<const CD3DX12_STATIC_SAMPLER_DESC, 6> ZekrosEngine::GetStaticSamplers() const {
			const CD3DX12_STATIC_SAMPLER_DESC pointWrap(
				0,
				D3D12_FILTER_MIN_MAG_MIP_POINT,
				D3D12_TEXTURE_ADDRESS_MODE_WRAP,
				D3D12_TEXTURE_ADDRESS_MODE_WRAP,
				D3D12_TEXTURE_ADDRESS_MODE_WRAP);

			const CD3DX12_STATIC_SAMPLER_DESC linearWrap(
				1,
				D3D12_FILTER_MIN_MAG_MIP_LINEAR,
				D3D12_TEXTURE_ADDRESS_MODE_WRAP,
				D3D12_TEXTURE_ADDRESS_MODE_WRAP,
				D3D12_TEXTURE_ADDRESS_MODE_WRAP);

			const CD3DX12_STATIC_SAMPLER_DESC pointClamp(
				2,
				D3D12_FILTER_MIN_MAG_MIP_POINT,
				D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
				D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
				D3D12_TEXTURE_ADDRESS_MODE_CLAMP);

			const CD3DX12_STATIC_SAMPLER_DESC linearClamp(
				3,
				D3D12_FILTER_MIN_MAG_MIP_LINEAR,
				D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
				D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
				D3D12_TEXTURE_ADDRESS_MODE_CLAMP);

			const CD3DX12_STATIC_SAMPLER_DESC anisotropicWrap(
				4,
				D3D12_FILTER_ANISOTROPIC,
				D3D12_TEXTURE_ADDRESS_MODE_WRAP,
				D3D12_TEXTURE_ADDRESS_MODE_WRAP,
				D3D12_TEXTURE_ADDRESS_MODE_WRAP,
				0.0f,
				8);

			const CD3DX12_STATIC_SAMPLER_DESC anisotropicClamp(
				5,
				D3D12_FILTER_ANISOTROPIC,
				D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
				D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
				D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
				0.0f,
				8);

			return { pointWrap, linearWrap, pointClamp, linearClamp, anisotropicWrap, anisotropicClamp };
		}

		ID3D12Resource* ZekrosEngine::SwapChainResource() const {
			return m_SwapChainBuffer[m_CurrentSwapBufferIndex].Get();
		}

		D3D12_CPU_DESCRIPTOR_HANDLE ZekrosEngine::CurrentBackBufferView() const {
			using orangelie::CppStdUtil::unmove;
			return unmove(CD3DX12_CPU_DESCRIPTOR_HANDLE(m_D3D12Interface->GetRtvDescriptorHeap()->GetCPUDescriptorHandleForHeapStart(),
				m_CurrentSwapBufferIndex, m_D3D12Interface->GetDsvDescriptorIncrementSize()));
		}

		D3D12_CPU_DESCRIPTOR_HANDLE ZekrosEngine::DepthStencilView() const {
			return m_D3D12Interface->GetDsvDescriptorHeap()->GetCPUDescriptorHandleForHeapStart();
		}
	}
}

orangelie::Engine::ZekrosEngine* orangelie::Engine::ZekrosEngine::gZekrosEngine = nullptr;
LRESULT _stdcall WindowProcedure(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam) {
	return orangelie::Engine::ZekrosEngine::gZekrosEngine->MessageHandler(hWnd, uMessage, wParam, lParam);
}