#pragma once
#pragma comment(lib, "d3d9")
#pragma comment(lib, "d3dx9")

#include "Win32.h"

#include <d3d9.h>
#include <d3dx9.h>

namespace orangelie {

	namespace d3d9framework {

		class d3d9app {
		public:
			d3d9app();
			d3d9app(const d3d9app&) = delete;
			d3d9app& operator=(const d3d9app&) = delete;
			~d3d9app();

			void Initialize(int screenWidth, int screenHeight, bool isFullscreen);
			void Render();
			void Frame();

			static d3d9app* gd3d9App;

		private:
			void init();
			void draw();

			std::vector<D3DMATERIAL9> m_Materials;
			std::vector<LPDIRECT3DTEXTURE9> m_Textures;
			ComPtr<ID3DXMesh> m_Mesh = nullptr;
			DWORD m_NumMaterials;

		private:
			std::unique_ptr<orangelie::Windows::Win32> m_Win32;

			ComPtr<IDirect3D9> m_Direct9 = nullptr;
			ComPtr<IDirect3DDevice9> m_Device = nullptr;

			UINT m_ClientWidth, m_ClientHeight;

		};
	}
}

LRESULT __stdcall CommandProcedure(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);