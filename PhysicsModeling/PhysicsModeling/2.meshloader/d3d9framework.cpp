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
			m_Device->SetRenderState(D3DRS_LIGHTING, TRUE);

			D3DLIGHT9 light;    // create the light struct

			ZeroMemory(&light, sizeof(light));    // clear out the struct for use
			light.Type = D3DLIGHT_DIRECTIONAL;    // make the light type 'directional light'
			light.Diffuse.r = 1.0f;    // .5 red
			light.Diffuse.g = 1.0f;    // .5 green
			light.Diffuse.b = 1.0f;    // .5 blue
			light.Diffuse.a = 1.0f;    // full alpha (we'll get to that soon)

			D3DVECTOR vecDirection = { -1.0f, -0.3f, -20.0f };    // the direction of the light
			light.Direction = vecDirection;    // set the direction

			m_Device->SetLight(0, &light);    // send the light struct properties to light #0
			m_Device->LightEnable(0, TRUE);    // turn on light #0

			init();
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
			D3DCOLOR color = D3DCOLOR_ARGB(255, 0, 0, 0);

			m_Device->Clear(
				0,
				nullptr,
				D3DCLEAR_STENCIL | D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
				color,
				1.0f,
				0);

			if (SUCCEEDED(m_Device->BeginScene())) {

				draw();

				m_Device->EndScene();
			}

			m_Device->Present(nullptr, nullptr, nullptr, nullptr);
		}

		void d3d9app::init() {
			const char* meshFilename = "./source/lugia.x";

			ComPtr<ID3DXBuffer> matBuffer = nullptr;
			DWORD numMaterial = 0;

			HR(D3DXLoadMeshFromX(
				meshFilename,
				D3DXMESH_MANAGED,
				m_Device.Get(),
				nullptr,
				matBuffer.GetAddressOf(),
				nullptr,
				&numMaterial,
				m_Mesh.GetAddressOf()
			));

			m_Materials.resize(numMaterial);
			m_Textures.resize(numMaterial);
			D3DXMATERIAL* MatPointer = (D3DXMATERIAL*)matBuffer->GetBufferPointer();


			for (DWORD i = 0; i < numMaterial; ++i) {
				m_Materials[i] = MatPointer[i].MatD3D;
				m_Materials[i].Ambient = m_Materials[i].Diffuse;

				if (MatPointer[i].pTextureFilename) {
					if (FAILED(D3DXCreateTextureFromFile(
						m_Device.Get(),
						"Lugia-TextureMap.jpg",		// does not work.... then i use a constant string
						&m_Textures[i]))) {

					}
				}
				else {

				}
			}

			m_NumMaterials = numMaterial;
		}

		void d3d9app::draw() {
			static float x = 0;

			D3DXMATRIXA16 world, view, projection;
			D3DXMatrixIdentity(&world); D3DXMatrixIdentity(&view); D3DXMatrixIdentity(&projection);

			D3DXVECTOR3 eye(2 * sinf(x) * cos(x), 0.0f, -10.0f);
			D3DXVECTOR3 lookat(0.0f, 0.0f, 1.0f);
			D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
			x += 0.01f;

			D3DXMatrixLookAtLH(&view, &eye, &lookat, &up);
			D3DXMatrixPerspectiveFovLH(
				&projection,
				D3DX_PI * 0.25f,
				(float)((float)m_ClientWidth / (float)m_ClientHeight),
				0.1f,
				1000.0f);

			m_Device->SetTransform(D3DTS_WORLD, &world);
			m_Device->SetTransform(D3DTS_VIEW, &view);
			m_Device->SetTransform(D3DTS_PROJECTION, &projection);



			for (DWORD i = 0; i < m_NumMaterials; ++i) {
				m_Device->SetMaterial(&m_Materials[i]);
				m_Device->SetTexture(0, m_Textures[i]);
				m_Mesh->DrawSubset(i);
			}
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