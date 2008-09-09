#include "stdafx.h"
#include <windows.h>
#include ".\Helpers.h"

CString GetNewFilename()
{
    CString strRet;
	SYSTEMTIME tNow;
	GetLocalTime(&tNow);
	strRet.Format(_T("bSDump-%u-%02u-%02u--%02u-%02u-%02u-%03u"), tNow.wYear, tNow.wMonth, tNow.wDay, tNow.wHour, tNow.wMinute, tNow.wSecond, tNow.wMilliseconds);
    return strRet;
}

/*
This was added for the region select dialog, if that ever gets implemented
void SetTransparency(HWND hWnd, long lAlpha)
{
	OSVERSIONINFO osv;
	osv.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osv);
	if(osv.dwMajorVersion < 5)
	{
		long oldStyle = GetWindowLong(hWnd, GWL_EXSTYLE);
        // TODO: Find out why WS_EX_LAYERED shows up as undefined
		SetWindowLong(hWnd, GWL_EXSTYLE, oldStyle | WS_EX_LAYERED);
        // TODO: Find out why LWA_ALPHA shows up as undefined
		SetLayeredWindowAttributes(hWnd, NULL, (BYTE)lAlpha, LWA_ALPHA);
	}
}
*/