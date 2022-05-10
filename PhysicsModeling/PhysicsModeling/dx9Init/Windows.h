/*
*
* < Windows.h >
* Github: https://github.com/orangelie
* // Copyright (C) 2022 by orangelie, Inc. All right reserved.
* // MIT License
*
*/





#pragma once

#if not defined(WIN32_LEAN_AND_MEAN)
#define WIN32_LEAN_AND_MEAN
#endif

#include <iostream>
#include <string>

#include <wrl.h>
#include <Windows.h>
#include <windowsx.h>

#define THROW(n) throw std::runtime_error(n)

using namespace Microsoft::WRL;

namespace orangelie {
	namespace Windows {
		class Win32 {
		public:
			Win32();
			Win32(const Win32&) = delete;
			Win32& operator=(const Win32&) = delete;
			~Win32();

			void Intialize(WNDPROC wndProcedureFunc, int maxScreenWidth, int maxScreenHeight, bool isFullscreenMode);

			HWND GetHwnd() const;

		private:
			void BuildWndClass(WNDPROC wndProcedureFunc);
			void BuildDevMode();
			void CreateParentProcess();

		private:
			const char*	m_ModuleClassName = "ZekrosEngine@orangelie";

			HWND			m_HwndHandle;
			HINSTANCE		m_MouduleHandle;

			int				m_MaxScreenWidth;
			int				m_MaxScreenHeight;
			int				m_IsFullscreenMode;

			int				m_DesktopScreenWidth;
			int				m_DesktopScreenHeight;
			int				m_WindowPositionX;
			int				m_WindowPositionY;
		};
	}
}