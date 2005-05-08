#include "stdafx.h"
#include "bScreenDumped2.h"
#include "bScreenDumped2Dlg.h"
#include "shellapi.h"
#include "Windows.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define SHELLICON_MSG (WM_USER + 100)

CbScreenDumped2Dlg::CbScreenDumped2Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CbScreenDumped2Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CbScreenDumped2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CbScreenDumped2Dlg, CDialog)
	ON_MESSAGE(SHELLICON_MSG, ShellIconCallback)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// CbScreenDumped2Dlg message handlers

BOOL CbScreenDumped2Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	ShellIcon_Initialize();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CbScreenDumped2Dlg::ShellIcon_Initialize()
{
	NOTIFYICONDATA ni;
	int iCount = 0;
	ni.cbSize = NOTIFYICONDATA_V1_SIZE; 
	ni.hWnd = m_hWnd; 
	ni.uID = 1;
	ni.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	ni.uCallbackMessage = SHELLICON_MSG;
	sprintf(ni.szTip,"bScreenDumped"); 
	ni.hIcon = m_hIcon;
	
	while ( (Shell_NotifyIcon(NIM_ADD, &ni) && (iCount <= 6 ) ) )
	{
        Sleep(150);
        iCount += 1;
	}

	if(iCount > 6)
	{
		MessageBox("Error while trying to create the SystemTray Icon. The program is still running, you may exit it using the Windows Task Manager utility.",
					"bScreenDumped->ShellIcon_Initialize()", MB_OK | MB_ICONINFORMATION);
	}
}

void CbScreenDumped2Dlg::ShellIcon_Terminate()
{
	NOTIFYICONDATA ni;
	ni.cbSize = NOTIFYICONDATA_V1_SIZE; 
	ni.hWnd = m_hWnd; 
	ni.uID = 1;
	Shell_NotifyIcon(NIM_DELETE, &ni);
}

LRESULT CbScreenDumped2Dlg::ShellIconCallback(WPARAM wParam, LPARAM lParam)
{
	if (lParam == WM_RBUTTONDOWN)
    {
		CMenu *PopUpMenu, mnu;
		mnu.LoadMenu(IDR_MENU1);
		PopUpMenu = mnu.GetSubMenu(0);
		CPoint pt;
		GetCursorPos(&pt);		
		SetForegroundWindow();
		PopUpMenu->TrackPopupMenu(TPM_CENTERALIGN,pt.x,pt.y,this);
		PostMessage(WM_NULL, 0, 0);
	}
	return 0;
}

BOOL CbScreenDumped2Dlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if(LOWORD(wParam) == IDM_EXIT)
    {
		ShellIcon_Terminate();
		EndDialog(1);
		return 1;
	}
	else
	{
		return 0;
	}
}