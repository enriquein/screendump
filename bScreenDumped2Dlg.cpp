#include "stdafx.h"
#include "HogVideo.h"
#include "bScreenDumped2.h"
#include "bScreenDumped2Dlg.h"
#include "OptionsDialog.h"
#include "shellapi.h"
#include "Windows.h"
#include "WindowCapture.h"
#include "GlobalSettings.h"
#include "gdiplus.h"
using namespace Gdiplus;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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
	if(Odlg != NULL)
	{
		Odlg->DestroyWindow();
	}
	EndDialog(1);
}

void CbScreenDumped2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CbScreenDumped2Dlg, CDialog)
	ON_MESSAGE(SHELLICON_MSG, ShellIconCallback)
	ON_MESSAGE(WM_HOTKEY, ProcessHotKey)
	ON_MESSAGE(WM_TIMER, OnTimer)
	ON_MESSAGE(OPTIONSDLG_CLOSED, OnOptDlgClose)
	ON_COMMAND(ID_TRAY_ABOUT, OnTrayAboutClick)
	ON_COMMAND(ID_TRAY_EXIT, OnTrayExitClick)
	ON_COMMAND(ID_TRAY_OPTIONS, OnTrayOptionsClick)
	ON_COMMAND(ID_TRAY_OPENDEST, OnTrayOpenDest)
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
		MessageBox("Error while trying to initialize GDI+. The program will quit now.\nDon't forget to contact me and tell me about this problem.",
					"bScreenDumped->MainDlg->OnInitDlg()", MB_OK | MB_ICONERROR);
		EndDialog(1);
	}
	Odlg = NULL;
	ShellIcon_Initialize();
	DoRegisterHotKeys();
	CGlobalSettings gs;
	gs.ReadSettings();
	if(gs.bEnableHog)
	{
		m_Hog.SetVideo("bs.dont.delete.me");
		m_Hog.Hog();
	}
	SetWindowPos(0,-100,-100,0,0, SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOOWNERZORDER);
	::SetTimer(m_hWnd, TIMERHIDE, 1000, NULL); 
	return TRUE;  // return TRUE  unless you set the focus to a control
}

LRESULT CbScreenDumped2Dlg::OnTimer(WPARAM wParam, LPARAM lParam)
{
	KillTimer(TIMERHIDE);
	SetWindowPos(0,0,0,0,0, SWP_NOSIZE | SWP_NOMOVE | SWP_HIDEWINDOW | SWP_NOACTIVATE | SWP_NOOWNERZORDER);
	return 0;
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
	sprintf(ni.szTip,"bScreenDumped v (rev )"); // FIXME
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

void CbScreenDumped2Dlg::OnTrayExitClick()
{
	DoCleanup();
}

void CbScreenDumped2Dlg::OnTrayAboutClick()
{
// FIXME
}

void CbScreenDumped2Dlg::OnTrayOpenDest()
{
	CGlobalSettings gs;
	gs.ReadSettings();
	ShellExecute(m_hWnd, "open", gs.szOutputDir, NULL, NULL, SW_SHOWNORMAL);
// FIXME
}

void CbScreenDumped2Dlg::OnTrayOptionsClick()
{
	if(Odlg == NULL)
	{
		Odlg = new OptionsDialog;
		Odlg->Create(IDD_SETTINGSDLG);
		Odlg->ShowWindow(SW_SHOWNORMAL);
		Odlg->parenthWnd = m_hWnd;
	}
	else
	{
		Odlg->ShowWindow(SW_SHOW);
	}
}

LRESULT CbScreenDumped2Dlg::OnOptDlgClose(WPARAM wParam, LPARAM lParam)
{
	Odlg = NULL;
	return 0;
}

void CbScreenDumped2Dlg::DoRegisterHotKeys()
{
	long result, resultAlt;
	m_Atom = new CGlobalAtom; 
	result = RegisterHotKey( m_hWnd, m_Atom->GetID(), 0, VK_SNAPSHOT);
	Sleep(50);
	m_AtomAlt = new CGlobalAtom;
	resultAlt = RegisterHotKey(m_hWnd, m_AtomAlt->GetID(), MOD_ALT, VK_SNAPSHOT);
	if( (result == 0) || (resultAlt == 0) )
	{
		MessageBox("Failed to register the hotkeys. This is almost always caused because of another program using the PrintScreen Key.\nPlease close the offending program before starting this one. This program will exit now.",
				   "bScreenDumped->DoRegisterHotKeys()", MB_OK | MB_ICONERROR);
		DoCleanup();
	}
	if( m_Atom->GetID() == m_AtomAlt->GetID() )
	{
		AfxMessageBox("Both keys got assigned the same Atom. This bug is still under investigation. Please restart the program.", MB_OK, 0);
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
		CaptureScreen();
	}
	else
	{
		if( wParam == m_AtomAlt->GetID() )
		{
			CaptureWindow();
		}
	}
	return 0;
}
