// Author: yangqi
/**
 * \file DPIUtils.h
 * \brief 
 * \author yangqi
 * \date June 2017
 *
 */
#pragma once
#include <windows.h>

namespace se
{
	// the wrapper of SetProcessDpiAwareness and SetProcessDPIAware
	inline void EnableDPIAware()
	{
		typedef enum _PROCESS_DPI_AWARENESS
		{
			PROCESS_DPI_UNAWARE = 0,
			PROCESS_SYSTEM_DPI_AWARE = 1,
			PROCESS_PER_MONITOR_DPI_AWARE = 2
		} PROCESS_DPI_AWARENESS;

		do
		{
			HMODULE hModule = ::LoadLibrary(L"Shcore.dll");
			if (hModule)
			{
				typedef HRESULT(WINAPI *SetProcessDpiAwareness)(PROCESS_DPI_AWARENESS);
				SetProcessDpiAwareness f = reinterpret_cast<SetProcessDpiAwareness>(::GetProcAddress(hModule, "SetProcessDpiAwareness"));
				if (f)
				{
					f(PROCESS_PER_MONITOR_DPI_AWARE);
					break;
				}
			}

			hModule = ::LoadLibrary(L"User32.dll");
			if (hModule)
			{
				typedef BOOL(WINAPI *SetProcessDPIAware)(void);
				SetProcessDPIAware f = reinterpret_cast<SetProcessDPIAware>(::GetProcAddress(hModule, "SetProcessDPIAware"));
				if (f)
				{
					f();
					break;
				}
			}
		} while (false);
	}
}
