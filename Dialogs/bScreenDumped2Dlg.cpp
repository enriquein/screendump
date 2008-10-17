#include "stdafx.h"
#include "..\Classes\HogVideo.h"
#include ".\bScreenDumped2Dlg.h"
#include "..\Classes\WindowCapture.h"
#include "..\Classes\GlobalSettings.h"
#include ".\OptionsDialog.h"
#include ".\AboutDialog.h"
#include ".\AutoCapture.h"
#include "..\Classes\file_ver.h"
#include "..\Classes\ErrorString.h"

UINT CbScreenDumped2Dlg::UWM_SHELLICON_MSG = ::RegisterWindowMessage(_T("UWM_SHELLICON_MSG-{7F1B3C8F-EAE9-4244-8D47-B6B2085F97EB}"));
UINT CbScreenDumped2Dlg::UWM_TOGGLETRAY = ::RegisterWindowMessage(_T("UWM_TOGGLETRAY-{963FEF79-2137-4fa7-A0D9-D1C4F1D32298}"));
UINT CbScreenDumped2Dlg::UWM_CAPTUREWINDOW = ::RegisterWindowMessage(_T("UWM_CAPTURESCREEN-{8BCA6B45-C3E5-4c08-8D1D-C6CF1CE4E6F0}"));
UINT CbScreenDumped2Dlg::UWM_CAPTURESCREEN = ::RegisterWindowMessage(_T("UWM_CAPTUREWINDOW-{15C4F437-8121-4530-BC07-FDB0E695012A}"));
UINT CbScreenDumped2Dlg::UWM_REQUESTHOG = ::RegisterWindowMessage(_T("UWM_REQUESTHOG-{E12B2B17-5A47-4691-B962-4469B1F960E6}"));

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CbScreenDumped2Dlg::CbScreenDumped2Dlg(CWnd* pParent /*=NULL*/) : CDialog(CbScreenDumped2Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    // Check if GDI+ was successfully loaded:
    try
    {
        wc = new WindowCapture();
    }
    catch(...)
    {
        CString msg;
        msg = _T("An error has ocurred while initializing the capture engine.\nThis is usually related to gdiplus.dll not being available or not loading correctly.\nPlease make sure you installed the program version that matches your version of Windows.");
        MessageBox(msg, _T("bScreenDumped->Ctor"), MB_OK | MB_ICONERROR);
        wc = NULL;
    }
}

CbScreenDumped2Dlg::~CbScreenDumped2Dlg()
{
    delete wc;
	DoUnregisterHotKeys();
	ShellIcon_Terminate(); 

}

void CbScreenDumped2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CbScreenDumped2Dlg, CDialog)
    ON_REGISTERED_MESSAGE(CbScreenDumped2Dlg::UWM_SHELLICON_MSG, ShellIconCallback)
    ON_REGISTERED_MESSAGE(CbScreenDumped2Dlg::UWM_TOGGLETRAY, OnToggleTrayMsg)
    ON_REGISTERED_MESSAGE(CbScreenDumped2Dlg::UWM_CAPTURESCREEN, OnCaptureScreenMsg)
    ON_REGISTERED_MESSAGE(CbScreenDumped2Dlg::UWM_CAPTUREWINDOW, OnCaptureWindowMsg)
    ON_REGISTERED_MESSAGE(CbScreenDumped2Dlg::UWM_REQUESTHOG, OnRequestHog)
	ON_MESSAGE(WM_HOTKEY, ProcessHotKey)
	ON_COMMAND(ID_TRAY_ABOUT, OnTrayAboutClick)
	ON_COMMAND(ID_TRAY_EXIT, OnTrayExitClick)
	ON_COMMAND(ID_TRAY_OPTIONS, OnTrayOptionsClick)
	ON_COMMAND(ID_TRAY_OPENDEST, OnTrayOpenDest)
	ON_COMMAND(ID_TRAY_AUTOCAPTURE, OnTrayAutoCapture)
	//}}AFX_MSG_MAP
    ON_WM_WINDOWPOSCHANGING()
END_MESSAGE_MAP()

BOOL CbScreenDumped2Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();
    
    if(wc == NULL)
    {
        // If we got here, then our windowcapture engine is dead. Bail.
        EndDialog(1);
        return TRUE;
    }

    CGlobalSettings gs;
    gs.ReadSettings();
    wc->SetEncoder(gs.sEnc, gs.lJpgQuality);
	CString exeName(CString(AfxGetAppName()) + CString(_T(".exe")));
	CFileVersionInfo cfInfo;
	cfInfo.ReadVersionInfo(exeName);
	if( cfInfo.IsValid() )
	{
		m_progVersion = cfInfo.GetCustomFileVersionString();
	}
	else
	{
		m_progVersion = _T(" ");
	}
	m_trayMenu.LoadMenu(IDR_MENU1); 
	ShellIcon_Initialize();
	DoRegisterHotKeys();
    StartHog();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CbScreenDumped2Dlg::ShellIcon_Initialize()
{
    CString ttipText;
	NOTIFYICONDATA ni;
	ni.cbSize = NOTIFYICONDATA_V1_SIZE; 
	ni.hWnd = m_hWnd; 
	ni.uID = 1;
	ni.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    ni.uCallbackMessage = UWM_SHELLICON_MSG;
    ttipText = _T("bScreenDumped ") + m_progVersion;
    _tcscpy_s(ni.szTip, ttipText);
	ni.hIcon = m_hIcon;
	
    // We have to keep retrying because Shell_NotifyIcon usually fails during Windows startup
    // on some machines with lots of other programs loading at startup as well.
    // We'll give it 30 seconds to consider some older machines running AOL/Norton etc.
	int iCount = 0;
    int maxCount = 30;
	while ( (Shell_NotifyIcon(NIM_ADD, &ni) && (iCount <= maxCount ) ) )
	{
        Sleep(1000);
        iCount += 1;
	}

	if(iCount > maxCount)
	{
		MessageBox(_T("Error while trying to create the System Tray Icon.\nThe program will continue to run in the background."),
					_T("bScreenDumped->ShellIcon_Initialize()"), MB_OK | MB_ICONINFORMATION);
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
		CPoint pt;
		GetCursorPos(&pt);		
		SetForegroundWindow();
		m_trayMenu.GetSubMenu(0)->TrackPopupMenu(TPM_CENTERALIGN,pt.x,pt.y,this);
		PostMessage(WM_NULL, 0, 0);
	}
	return 0;
}

void CbScreenDumped2Dlg::OnTrayExitClick()
{
	EndDialog(1);
}

void CbScreenDumped2Dlg::OnTrayAboutClick()
{
	CAboutDialog cAbt;
	ToggleTrayMenu(FALSE);
	cAbt.DoModal();
	ToggleTrayMenu(TRUE);
}

void CbScreenDumped2Dlg::OnTrayOpenDest()
{
	CGlobalSettings gs;
	gs.ReadSettings();
	ShellExecute(m_hWnd, _T("open"), gs.getOutputDir(), NULL, NULL, SW_SHOWNORMAL);
}

void CbScreenDumped2Dlg::OnTrayOptionsClick()
{
	OptionsDialog oDl;
	ToggleTrayMenu(FALSE);
	oDl.DoModal();
	ToggleTrayMenu(TRUE);
}

void CbScreenDumped2Dlg::OnTrayAutoCapture()
{
	CAutoCapture cAC;
	ToggleTrayMenu(FALSE);
	cAC.DoModal();
	ToggleTrayMenu(TRUE);
}

// Allows other dialogs to request Screen Captures.
LRESULT CbScreenDumped2Dlg::OnCaptureScreenMsg(WPARAM wParam, LPARAM lParam)
{
    RequestScreenCapture();
    return 0;
}

// Allows other dialogs to request Window Captures 
LRESULT CbScreenDumped2Dlg::OnCaptureWindowMsg(WPARAM wParam, LPARAM lParam)
{
    RequestWindowCapture();
    return 0;
}

// Allows other dialogs to request enabling/disabling of the hog interface.
// (BOOL) wParam = Should we enable/disable hog? True means Enable, False disable.
LRESULT CbScreenDumped2Dlg::OnRequestHog(WPARAM wParam, LPARAM lParam)
{
    if( (BOOL)wParam )
    {
        StartHog();
    }
    else
    {
        StopHog();
    }
    return 0;
}

// (BOOL)wParam tells us if we want to enable/disable the tray. TRUE = enable, FALSE = disable.
LRESULT CbScreenDumped2Dlg::OnToggleTrayMsg(WPARAM wParam, LPARAM lParam)
{
    if(wParam != NULL)
    {
        ToggleTrayMenu((BOOL)wParam);
    }
    return 0;
}

void CbScreenDumped2Dlg::DoRegisterHotKeys()
{
	long result, resultAlt;
	m_Atom = new CGlobalAtom; 
	result = RegisterHotKey( m_hWnd, m_Atom->GetID(), 0, VK_SNAPSHOT);
	Sleep(150); // Since we use the system time to initialize the atoms, we need to wait to avoid getting the same timestamp.
	m_AtomAlt = new CGlobalAtom;
	resultAlt = RegisterHotKey(m_hWnd, m_AtomAlt->GetID(), MOD_ALT, VK_SNAPSHOT);
	if( (result == 0) || (resultAlt == 0) )
	{
		MessageBox(_T("Failed to register the hotkeys. This is almost always caused because of another program using the PrintScreen Key.\nPlease close the offending program before restarting this one."),
				   _T("bScreenDumped->DoRegisterHotKeys()"), MB_OK | MB_ICONERROR);
	}
}

void CbScreenDumped2Dlg::DoUnregisterHotKeys()
{
    if( (m_Atom != NULL) && (m_AtomAlt != NULL) )
    {
        UnregisterHotKey( m_hWnd, m_Atom->GetID() );
        UnregisterHotKey( m_hWnd, m_AtomAlt->GetID() );
        delete m_Atom;
	    delete m_AtomAlt;
    }
}

LRESULT CbScreenDumped2Dlg::ProcessHotKey(WPARAM wParam, LPARAM lParam)
{
    if( wParam == m_Atom->GetID() )
    {
        RequestScreenCapture();
    }
    else
    {
        if( wParam == m_AtomAlt->GetID() )
        {
	        RequestWindowCapture(); 
        }
    }    
	return 0;
}

void CbScreenDumped2Dlg::ToggleTrayMenu(BOOL bEnable)
{	
	UINT lFlags;
	if(bEnable)
	{
		lFlags = MF_BYCOMMAND|MF_ENABLED;
	}
	else
	{
		lFlags = MF_BYCOMMAND|MF_DISABLED|MF_GRAYED;
	}
	m_trayMenu.GetSubMenu(0)->EnableMenuItem(ID_TRAY_ABOUT, lFlags);
	m_trayMenu.GetSubMenu(0)->EnableMenuItem(ID_TRAY_EXIT, lFlags);
	m_trayMenu.GetSubMenu(0)->EnableMenuItem(ID_TRAY_OPENDEST, lFlags);
	m_trayMenu.GetSubMenu(0)->EnableMenuItem(ID_TRAY_OPTIONS, lFlags);
	m_trayMenu.GetSubMenu(0)->EnableMenuItem(ID_TRAY_AUTOCAPTURE, lFlags);
}

// Starts the hog video window. 
// Auto verifies if the setting is enabled before doing any work.
void CbScreenDumped2Dlg::StartHog()
{
    CGlobalSettings gs;
	gs.ReadSettings();
    if (gs.bEnableHog)
    {
        CString strFileName(_T("bs.dont.delete.me"));
        m_Hog.SetVideo(strFileName);
        BOOL tmpSuccess = TRUE;
        CString tmpErrStr;
        if(_taccess(strFileName, 0) == -1)
        {
            tmpErrStr = _T("A problem ocurred while enabling screenshots of videos:\nCould not find the `bs.dont.delete.me` file inside the program folder.\nThis option will be disabled for now.");
            tmpSuccess = FALSE;
        }
        else
        {
	        if(!m_Hog.Hog())
            {
                tmpErrStr = _T("A problem ocurred while enabling screenshots of videos.\nPlease report this message.\nThis option will be disabled for now.");
                tmpSuccess = FALSE;
            }
        }
        if(!tmpSuccess)
        {
		    MessageBox(tmpErrStr, _T("bScreenDumped->InitDialog()->EnablingHog"), MB_OK|MB_ICONERROR);
		    gs.bEnableHog = FALSE;
		    gs.WriteSettings();
	    }
    }
}

// Stops the hog window. 
void CbScreenDumped2Dlg::StopHog()
{
    m_Hog.UnHog();
}

void CbScreenDumped2Dlg::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
    lpwndpos->flags &= ~SWP_SHOWWINDOW;
    CDialog::OnWindowPosChanging(lpwndpos);
}

void CbScreenDumped2Dlg::RequestWindowCapture()
{
    // I really hate duplicating code, just seemed necessary here
    CGlobalSettings gs;
    CString fName;
    try
    {
        fName = gs.GetNewFileName();
    }
    catch (CFileException* e)
    {
        if(e->m_cause == CFileException::badPath)
        {
            CString errMsg;
            errMsg.Format(_T("There was an error trying to save the image to %s. Possible reasons are:\nPath is invalid, access is denied, folder or drive is read only."), fName);
            MessageBox(errMsg, _T("bScreenDumped->RequestCapture"), MB_ICONERROR|MB_OK);
        }
    }
    if(fName.GetLength() > 0)
    {
        wc->CaptureWindow(fName);
    }
}

void CbScreenDumped2Dlg::RequestScreenCapture()
{
    // I really hate duplicating code, just seemed necessary here
    CGlobalSettings gs;
    CString fName;
    try
    {
        fName = gs.GetNewFileName();
    }
    catch (CFileException* e)
    {
        if(e->m_cause == CFileException::badPath)
        {
            CString errMsg;
            errMsg.Format(_T("There was an error trying to save the image to %s. Possible reasons are:\nPath is invalid, access is denied, folder or drive is read only."), fName);
            MessageBox(errMsg, _T("bScreenDumped->RequestCapture"), MB_ICONERROR|MB_OK);            
        }
    }
    if(fName.GetLength() > 0)
    {
        wc->CaptureScreen(fName);
    }
}