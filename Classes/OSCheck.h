#pragma once
#include "..\stdafx.h"
#include <windows.h>
#include "dwmapi.h"

class OSCheck
{
public:
    static BOOL CheckAeroIsEnabled();
    static int GetMajorOSVersion();
    static CString GetOSVersion();
    
protected:
    static OSVERSIONINFO GetOSVersionInfo();
};

inline BOOL OSCheck::CheckAeroIsEnabled()
{
    if(GetMajorOSVersion() < 6) // 6 is Vista
        return FALSE;
	    
    BOOL isCompositionEnabled = FALSE;
    HRESULT callResult = DwmIsCompositionEnabled(&isCompositionEnabled);     
    return (isCompositionEnabled) && (callResult == S_OK);
}

inline int OSCheck::GetMajorOSVersion()
{
    return GetOSVersionInfo().dwMajorVersion;    
}

inline CString OSCheck::GetOSVersion()
{
    return GetOSVersionInfo().szCSDVersion;
}

inline OSVERSIONINFO OSCheck::GetOSVersionInfo()
{
    OSVERSIONINFO osv;
    osv.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&osv);
    return osv;
}