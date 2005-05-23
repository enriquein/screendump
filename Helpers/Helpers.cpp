#include "stdafx.h"
#include ".\Helpers.h"

// must pass reference to a char[37] (or more) variable or it will shit hair!
void GetNewFilename(char* retVal)
{
	SYSTEMTIME tNow;
	GetLocalTime(&tNow);
	sprintf(retVal, "bSDump-%u-%02u-%02u--%02u-%02u-%02u-%03u", tNow.wYear, tNow.wMonth, tNow.wDay, tNow.wHour, tNow.wMinute, tNow.wSecond, tNow.wMilliseconds);
}

void ToggleTrayMenu(BOOL bEnable)
{	
	CbScreenDumped2Dlg* mainWnd;
	UINT lFlags;
	if(bEnable)
	{
		lFlags = MF_BYCOMMAND|MF_ENABLED;
	}
	else
	{
		lFlags = MF_BYCOMMAND|MF_DISABLED|MF_GRAYED;
	}
	mainWnd = (CbScreenDumped2Dlg*)AfxGetApp()->m_pMainWnd;
	mainWnd->m_trayMenu->EnableMenuItem(ID_TRAY_ABOUT, lFlags);
	mainWnd->m_trayMenu->EnableMenuItem(ID_TRAY_EXIT, lFlags);
	mainWnd->m_trayMenu->EnableMenuItem(ID_TRAY_OPENDEST, lFlags);
	mainWnd->m_trayMenu->EnableMenuItem(ID_TRAY_OPTIONS, lFlags);
	mainWnd->m_trayMenu->EnableMenuItem(ID_TRAY_AUTOCAPTURE, lFlags);
}