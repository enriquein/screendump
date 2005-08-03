#ifndef MY_HELPERS
#define MY_HELPERS

#include "..\stdafx.h"
#include "Windows.h"
#include "..\Dialogs\bScreenDumped2Dlg.h"

void SetTransparency(HWND hWnd, long lAlpha); 
void GetNewFilename(char* retVal);
void ToggleTrayMenu(BOOL bEnable);
#endif
