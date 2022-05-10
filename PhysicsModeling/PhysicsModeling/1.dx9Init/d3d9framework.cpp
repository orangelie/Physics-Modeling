#include "d3d9framework.h"

namespace orangelie {

	namespace d3d9framework {

		d3d9app::d3d9app() {
			gd3d9App = this;
		}

		d3d9app::~d3d9app() {
		}

		void d3d9app::Initialize(int screenWidth, int screenHeight, bool isFullscreen) {
			m_Win32 = std::make_unique<orangelie::Windows::Win32>();
			m_Win32->Intialize(CommandProcedure, screenWidth, screenHeight, isFullscreen);

			m_Direct9 = Direct3DCreate9(D3D_SDK_VERSION);
			if (m_Direct9 == nullptr)
				HR(-1);

			D3DPRESENT_PARAMETERS d3dPresentParam = {};
			d3dPresentParam.BackBufferCount = 2;
			d3dPresentParam.BackBufferFormat = D3DFMT_A8R8G8B8;
			d3dPresentParam.BackBufferHeight = screenHeight;
			d3dPresentParam.BackBufferWidth = screenWidth;
			d3dPresentParam.EnableAutoDepthStencil = TRUE;
			d3dPresentParam.AutoDepthStencilFormat = D3DFMT_D24S8;
			d3dPresentParam.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
			d3dPresentParam.hDeviceWindow = m_Win32->GetHwnd();
			d3dPresentParam.SwapEffect = D3DSWAPEFFECT_DISCARD;
			d3dPresentParam.Windowed = !isFullscreen;

			HR(m_Direct9->CreateDevice(
				D3DADAPTER_DEFAULT,
				D3DDEVTYPE_HAL,
				m_Win32->GetHwnd(),
				D3DCREATE_HARDWARE_VERTEXPROCESSING,
				&d3dPresentParam,
				m_Device.GetAddressOf()));

			m_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			m_Device->SetRenderState(D3DRS_LIGHTING, FALSE);
		}

		void d3d9app::Render() {
			MSG msg = {};

			for (;;) {
				if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
					DispatchMessage(&msg);
					TranslateMessage(&msg);

					if (msg.message == WM_QUIT) {
						break;
					}
				}
				else {
					Frame();
				}
			}
		}

		void d3d9app::Frame() {
			D3DCOLOR color = D3DCOLOR_ARGB(255, 0, 255, 255);

			m_Device->Clear(
				0,
				nullptr,
				D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
				color,
				1.0f,
				0);

			if (SUCCEEDED(m_Device->BeginScene())) {

				m_Device->EndScene();
			}

			m_Device->Present(nullptr, nullptr, nullptr, nullptr);
		}
	}
}

orangelie::d3d9framework::d3d9app* orangelie::d3d9framework::d3d9app::gd3d9App = nullptr;
LRESULT __stdcall CommandProcedure(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
	switch (Msg) {


	case WM_DESTROY:
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, Msg, wParam, lParam);
}