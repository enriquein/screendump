#include "stdafx.h"
#include "bScreenDumped2.h"
#include "bScreenDumped2Dlg.h"
#include "shellapi.h"
#include "Windows.h"
#include "WindowCapture.h"
#include "gdiplus.h"
using namespace Gdiplus;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define SHELLICON_MSG (WM_USER + 100)

CbScreenDumped2Dlg::CbScreenDumped2Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CbScreenDumped2Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CbScreenDumped2Dlg::DoCleanup()
{
	GdiplusShutdown(m_gdiPlusToken);
	DoUnregisterHotKeys();
	ShellIcon_Terminate();
	EndDialog(1);
}

void CbScreenDumped2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CbScreenDumped2Dlg, CDialog)
	ON_MESSAGE(SHELLICON_MSG, ShellIconCallback)
	ON_MESSAGE(WM_HOTKEY, ProcessHotKey)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// CbScreenDumped2Dlg message handlers

BOOL CbScreenDumped2Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	Status st;
	st = GdiplusStartup(&m_gdiPlusToken, &m_gdiPlusStatupInput, NULL );
	if(st != Ok)
	{
		MessageBox("death");
		EndDialog(1);
	}
	ShellIcon_Initialize();
	DoRegisterHotKeys();
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
	sprintf(ni.szTip,"bScreenDumped v (rev )"); 
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
		DoCleanup();
		return 1;
	}
	else
	{
		return 0;
	}
}

void CbScreenDumped2Dlg::DoRegisterHotKeys()
{
	long result, resultAlt;
	m_Atom = new CGlobalAtom; 
	Sleep(10);
	m_AtomAlt = new CGlobalAtom;
	result = RegisterHotKey( m_hWnd, m_Atom->GetID(), 0, VK_SNAPSHOT);
	resultAlt = RegisterHotKey(m_hWnd, m_AtomAlt->GetID(), MOD_ALT, VK_SNAPSHOT);
	if( (result == 0) || (resultAlt == 0) )
	{
		MessageBox("Failed to register the hotkeys. The program will exit now.", "bScreenDumped->DoRegisterHotKeys()", MB_OK | MB_ICONINFORMATION);
		DoCleanup();
	}
}

void CbScreenDumped2Dlg::DoUnregisterHotKeys()
{
    UnregisterHotKey( m_hWnd, m_Atom->GetID() );
    UnregisterHotKey( m_hWnd, m_AtomAlt->GetID() );
	delete m_Atom;
	delete m_AtomAlt;
}

LRESULT CbScreenDumped2Dlg::ProcessHotKey(WPARAM wParam, LPARAM lParam)
{
	if( wParam == m_Atom->GetID() )
	{
		if(!( CaptureWindow() ) )
		{
			AfxMessageBox("meh failed");
		}
	}
	else
	{
		if( wParam == m_AtomAlt->GetID() )
		{
			AfxMessageBox("Alt Print Screen!");
		}
	}
	return 0;
}