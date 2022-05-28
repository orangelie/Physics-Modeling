/*
*
* < ZekrosEngine.h >
* Github: https://github.com/orangelie
* // Copyright (C) 2022 by orangelie, Inc. All right reserved.
* // MIT License
*
*/





#pragma once

#include "../Windows/Windows.h"
#include "../Time/GameTimer.h"
#include "FrameResources.h"
#include "../DxInterface/InterfeceDxgi.h"
#include "../DxInterface/InterfaceD3D12.h"
#include "../Shader/ShaderSystem.h"
#include "../Mesh/MeshTools.h"
#include "../Camera/DefaultCamera.h"
#include "../Rendering/RenderSystem.h"
#include "../Mesh/GeometryGenerator.h"
#include "../Lighting/LightingUtils.h"
#include "../Texture/TextureTools.h"
#include "../Font/FontEngine.h"
#include "../Terrain/HeightMapTerrain.h"


namespace orangelie {
	namespace Engine {
		class ZekrosEngine {
		public:
			ZekrosEngine();
			ZekrosEngine(const ZekrosEngine&) = delete;
			ZekrosEngine& operator=(const ZekrosEngine&) = delete;
			~ZekrosEngine();

			void Initialize(int maxScreenWidth, int maxScreenHeight, bool isFullscreenMode);
			void Run();
			ID3D12Device* GeDevice() { return m_D3D12Interface->GetDevice(); };

			static ZekrosEngine* gZekrosEngine;
			virtual LRESULT MessageHandler(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);

		private:
			void CreateCommandObjects();

		protected:
			// Static Samplers
			std::array<const CD3DX12_STATIC_SAMPLER_DESC, 6> GetStaticSamplers() const;
			
			// Resource View & Resource
			ID3D12Resource* SwapChainResource() const;
			D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView() const;
			D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView() const;

			// SwapChain & CommandQueue Tool
			void PresentSwapChain();
			void SubmitCommandList();
			void FlushCommandQueue();

			// Screen Resize
			virtual void OnResize();

			// Virtuals
			virtual void init() = 0;
			virtual void update(float dt) = 0;
			virtual void draw(float dt) = 0;

			// Mouse Controller
			virtual void MouseDown(WPARAM btnState, int x, int y) = 0;
			virtual void MouseUp(WPARAM btnState, int x, int y) = 0;
			virtual void MouseMove(WPARAM btnState, int x, int y, float dt) = 0;

			// Client ScreenSize
			int m_ClientWidth;
			int m_ClientHeight;

			// Direct3D 12 Device
			ID3D12Device* m_Device = nullptr;

			// Command Objects
			ComPtr<ID3D12CommandAllocator> m_CommandAllocator;
			ComPtr<ID3D12GraphicsCommandList> m_CommandList;
			ComPtr<ID3D12CommandQueue> m_CommandQueue;

			// Viewport & ScissorRect
			D3D12_VIEWPORT m_Viewport;
			RECT m_ScissorRect;

			// BackBuffer
			int m_CurrentSwapBufferIndex = 0;
			static const int gBackBufferCount = 2;

			// FrameResources
			static const size_t gFrameResourceCount = 3;
			std::vector<std::unique_ptr<orangelie::FrameResource>> m_FrameResources;
			orangelie::FrameResource* m_CurrFrameResource;
			size_t m_CurrFrameResourceIndex = 0;

			// Resource Fence
			ID3D12Fence* m_Fence = nullptr;
			UINT64 m_CurrentFenceCount = 0;

			// format
			const DXGI_FORMAT m_BackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
			const DXGI_FORMAT m_DepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

			// hwnd
			HWND m_hWnd = nullptr;

			// Descriptor Sizes
			UINT m_RtvSize;
			UINT m_DsvSize;
			UINT m_CbvSrvUavSize;

		private:
			std::unique_ptr<orangelie::Windows::Win32> m_Win32 = nullptr;
			orangelie::Time::GameTimer m_GameTimer;
			std::unique_ptr<orangelie::DxInterface::InterfaceDxgi> m_DxgiInterface = nullptr;
			std::unique_ptr<orangelie::DxInterface::InterfaceD3D12> m_D3D12Interface = nullptr;

			bool m_IsEnginePaused = false;
			bool m_IsSizeMinimized = false;
			bool m_IsSizeMaximized = false;
			bool m_IsResizing = false;

			ComPtr<ID3D12Resource> m_SwapChainBuffer[gBackBufferCount];
			ComPtr<ID3D12Resource> m_DepthStencilBuffer = nullptr;
		};
	}
}

LRESULT _stdcall WindowProcedure(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam);