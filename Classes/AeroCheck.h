#pragma once
#ifndef AEROCHECK_H
#define AEROCHECK_H
#include "dwmapi.h"

BOOL CheckAeroIsEnabled()
{
    OSVERSIONINFO osv;
	osv.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osv);
	if(osv.dwMajorVersion < 6) // 6 is Vista
	    return FALSE;
	    
	BOOL isCompositionEnabled = FALSE;
	HRESULT callResult = DwmIsCompositionEnabled(&isCompositionEnabled);     
	return (isCompositionEnabled) && (callResult == S_OK);
}
#endif