#include "stdafx.h"
#include "bScreenDumped2.h"
#include "bScreenDumped2Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CbScreenDumped2App, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

CbScreenDumped2App::CbScreenDumped2App()
{
}

CbScreenDumped2App theApp;

BOOL CbScreenDumped2App::InitInstance()
{
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
