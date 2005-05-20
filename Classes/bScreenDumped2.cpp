#include "stdafx.h"
#include "bScreenDumped2.h"
#include "bScreenDumped2Dlg.h"
#include "LimitSingleInstance.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CbScreenDumped2App, CWinApp)
END_MESSAGE_MAP()

CbScreenDumped2App::CbScreenDumped2App()
{
}

CbScreenDumped2App theApp;
CLimitSingleInstance g_SingleInstanceObj(TEXT("Global\\{981AEB41-4426-4839-B27D-97F6E872204C}"));
// {981AEB41-4426-4839-B27D-97F6E872204C} Generated via GUIDGEN

BOOL CbScreenDumped2App::InitInstance()
{
	// Is another instance running?
	if (g_SingleInstanceObj.IsAnotherInstanceRunning())
	{
		AfxMessageBox("bScreenDumped is already running.", MB_OK|MB_ICONERROR, 0);
		return FALSE;
	}
	// InitCommonControls() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	InitCommonControls();
	CWinApp::InitInstance();
	CbScreenDumped2Dlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();
	return FALSE;
}
