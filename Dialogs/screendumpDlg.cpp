#include "stdafx.h"
#include "..\Classes\HogVideo.h"
#include ".\screendumpDlg.h"
#include "..\Classes\WindowCapture.h"
#include "..\Classes\GlobalSettings.h"
#include ".\OptionsDialog.h"
#include ".\AboutDialog.h"
#include ".\AutoCapture.h"
#include "..\Classes\file_ver.h"
#include "..\Classes\ErrorString.h"

UINT screendumpDlg::UWM_SHELLICON_MSG = ::RegisterWindowMessage(_T("UWM_SHELLICON_MSG-{7F1B3C8F-EAE9-4244-8D47-B6B2085F97EB}"));
UINT screendumpDlg::UWM_TOGGLETRAY = ::RegisterWindowMessage(_T("UWM_TOGGLETRAY-{963FEF79-2137-4fa7-A0D9-D1C4F1D32298}"));
UINT screendumpDlg::UWM_CAPTUREWINDOW = ::RegisterWindowMessage(_T("UWM_CAPTURESCREEN-{8BCA6B45-C3E5-4c08-8D1D-C6CF1CE4E6F0}"));
UINT screendumpDlg::UWM_CAPTURESCREEN = ::RegisterWindowMessage(_T("UWM_CAPTUREWINDOW-{15C4F437-8121-4530-BC07-FDB0E695012A}"));
UINT screendumpDlg::UWM_REQUESTVERSION = ::RegisterWindowMessage(_T("UWM_REQUESTVERSION-{F9264F49-8BFF-4667-8C00-9B9E8E9D0485}"));

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

screendumpDlg::screendumpDlg(CWnd* pParent /*=NULL*/) : CDialog(screendumpDlg::IDD, pParent)
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
        MessageBox(msg, _T("screendump->Ctor"), MB_OK | MB_ICONERROR);
        wc = NULL;
    }

	// Get our path
	GetModuleFileName(NULL, m_runningPath.GetBuffer(MAX_PATH), MAX_PATH * sizeof(TCHAR)); // MSDN says it asks for the size in TCHARs, and not char count.
	m_runningPath.ReleaseBuffer();
	m_runningPath = m_runningPath.Left(m_runningPath.ReverseFind(_T('\\')) + 1);
}

screendumpDlg::~screendumpDlg()
{
	if (wc != NULL)
	    delete wc;

	if (m_Atom != NULL)
		delete m_Atom;

	if (m_AtomAlt != NULL)
		delete m_AtomAlt;
}

void screendumpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(screendumpDlg, CDialog)
    ON_REGISTERED_MESSAGE(screendumpDlg::UWM_SHELLICON_MSG, ShellIconCallback)
    ON_REGISTERED_MESSAGE(screendumpDlg::UWM_TOGGLETRAY, OnToggleTrayMsg)
    ON_REGISTERED_MESSAGE(screendumpDlg::UWM_CAPTURESCREEN, OnCaptureScreenMsg)
    ON_REGISTERED_MESSAGE(screendumpDlg::UWM_CAPTUREWINDOW, OnCaptureWindowMsg)
    ON_REGISTERED_MESSAGE(screendumpDlg::UWM_REQUESTVERSION, OnRequestVersion)
	ON_MESSAGE(WM_HOTKEY, ProcessHotKey)
	ON_COMMAND(ID_TRAY_ABOUT, OnTrayAboutClick)
	ON_COMMAND(ID_TRAY_EXIT, OnTrayExitClick)
	ON_COMMAND(ID_TRAY_OPTIONS, OnTrayOptionsClick)
	ON_COMMAND(ID_TRAY_OPENDEST, OnTrayOpenDest)
	ON_COMMAND(ID_TRAY_AUTOCAPTURE, OnTrayAutoCapture)
	//}}AFX_MSG_MAP
    ON_WM_WINDOWPOSCHANGING()
	ON_WM_QUERYENDSESSION()
	ON_WM_CLOSE()
    ON_COMMAND(ID_TRAY_EMPTYCLIPBOARD, &screendumpDlg::OnTrayEmptyclipboard)
END_MESSAGE_MAP()

BOOL screendumpDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
    if(wc == NULL)
    {
        // If we got here, then our windowcapture engine is dead. Bail.
		MessageBox(_T("The capture engine failed to initialize. There might be something odd happening to the GDI+ dll. Exiting."), _T("Initialization Error"), MB_ICONSTOP | MB_OK);
        EndDialog(1);
        return TRUE;
    }
    RefreshSettings();
	CString exeName(CString(AfxGetAppName()) + CString(_T(".exe")));
	FileVersionInfo cfInfo;
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
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void screendumpDlg::ShellIcon_Initialize()
{
    CString ttipText;
	NOTIFYICONDATA ni;
	ni.cbSize = NOTIFYICONDATA_V1_SIZE; 
	ni.hWnd = m_hWnd; 
	ni.uID = 1;
	ni.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    ni.uCallbackMessage = UWM_SHELLICON_MSG;
    ttipText = _T("screendump ") + m_progVersion;
    _tcscpy_s(ni.szTip, ttipText);
	ni.hIcon = AfxGetApp()->LoadIcon(IDI_ICONTRAY);;
	
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
					_T("screendump->ShellIcon_Initialize()"), MB_OK | MB_ICONINFORMATION);
	}
}

void screendumpDlg::ShellIcon_Terminate()
{
	NOTIFYICONDATA ni;
	ni.cbSize = NOTIFYICONDATA_V1_SIZE; 
	ni.hWnd = m_hWnd; 
	ni.uID = 1;
	Shell_NotifyIcon(NIM_DELETE, &ni);
}

LRESULT screendumpDlg::ShellIconCallback(WPARAM wParam, LPARAM lParam)
{
    CPoint pt;
    switch (lParam)
    {
	    case (WM_RBUTTONDOWN):
		    GetCursorPos(&pt);		
		    SetForegroundWindow();
		    m_trayMenu.GetSubMenu(0)->TrackPopupMenu(TPM_CENTERALIGN,pt.x,pt.y,this);
		    PostMessage(WM_NULL, 0, 0);	
	        break;
	    case (WM_LBUTTONDOWN):
	    case (WM_RBUTTONDBLCLK):
	    case (WM_LBUTTONDBLCLK):
	        // We set foreground window in case we have an open dialog. We don't want to confuse the user as to why 
	        // the tray's menu is disabled at this point.
	   	    SetForegroundWindow();  
	   	    break;
	}
	return 0;
}

void screendumpDlg::OnTrayExitClick()
{
	OnClose();
}

void screendumpDlg::OnTrayAboutClick()
{
	AboutDialog cAbt((CWnd*)this);
	ToggleTrayMenu(FALSE);
	cAbt.DoModal();
	ToggleTrayMenu(TRUE);
}

void screendumpDlg::OnTrayOpenDest()
{
	GlobalSettings gs;
	ShellExecute(m_hWnd, _T("open"), gs.getOutputDir(), NULL, NULL, SW_SHOWNORMAL);
}

void screendumpDlg::OnTrayOptionsClick()
{
	OptionsDialog oDl((CWnd*)this);
	ToggleTrayMenu(FALSE);
	oDl.DoModal();
    RefreshSettings();
	ToggleTrayMenu(TRUE);
}

void screendumpDlg::OnTrayAutoCapture()
{
	AutoCapture cAC((CWnd*)this);
	ToggleTrayMenu(FALSE);
	cAC.DoModal();
	ToggleTrayMenu(TRUE);
}

// Allows other dialogs to request Screen Captures.
LRESULT screendumpDlg::OnCaptureScreenMsg(WPARAM wParam, LPARAM lParam)
{
    RequestCapture(ct_Screen);
    return 0;
}

// Allows other dialogs to request Window Captures 
LRESULT screendumpDlg::OnCaptureWindowMsg(WPARAM wParam, LPARAM lParam)
{
    RequestCapture(ct_Window);
    return 0;
}

void screendumpDlg::RefreshSettings()
{
    GlobalSettings gs;
    wc->SetEncoder(gs.sEnc, gs.lJpgQuality);
    wc->SetUseClipboard(gs.bWantClipboard);
    StartHog();
}

// (BOOL)wParam tells us if we want to enable/disable the tray. TRUE = enable, FALSE = disable.
LRESULT screendumpDlg::OnToggleTrayMsg(WPARAM wParam, LPARAM lParam)
{
    if(wParam != NULL)
    {
        ToggleTrayMenu((BOOL)wParam);
    }
    return 0;
}

// (CString *)(LRESULT) returns a pointer to a CString object that contains our version number.
LRESULT screendumpDlg::OnRequestVersion(WPARAM wParam, LPARAM lParam)
{
    return lParam = (LRESULT) &m_progVersion;
}

void screendumpDlg::DoRegisterHotKeys()
{
	long result, resultAlt;
	m_Atom = new GlobalAtom; 
	result = RegisterHotKey( m_hWnd, m_Atom->GetID(), 0, VK_SNAPSHOT);
	Sleep(150); // Since we use the system time to initialize the atoms, we need to wait to avoid getting the same timestamp.
	m_AtomAlt = new GlobalAtom;
	resultAlt = RegisterHotKey(m_hWnd, m_AtomAlt->GetID(), MOD_ALT, VK_SNAPSHOT);
	if( (result == 0) || (resultAlt == 0) )
	{
		MessageBox(_T("Failed to register the hotkeys. This is almost always caused because of another program using the PrintScreen Key.\nPlease close the offending program before restarting this one."),
				   _T("screendump->DoRegisterHotKeys()"), MB_OK | MB_ICONERROR);
	}
}

void screendumpDlg::DoUnregisterHotKeys()
{
    if(m_Atom != NULL)  
    {
        UnregisterHotKey( m_hWnd, m_Atom->GetID() );
        delete m_Atom;
		m_Atom = NULL;
    }
    
    if (m_AtomAlt != NULL) 
    {
        UnregisterHotKey( m_hWnd, m_AtomAlt->GetID() );    
	    delete m_AtomAlt;    
		m_AtomAlt = NULL;
    }
}

LRESULT screendumpDlg::ProcessHotKey(WPARAM wParam, LPARAM lParam)
{
    if( wParam == m_Atom->GetID() )
    {
        RequestCapture(ct_Screen);
    }
    else
    {
        if( wParam == m_AtomAlt->GetID() )
        {
	        RequestCapture(ct_Window); 
        }
    }    
	return 0;
}

void screendumpDlg::ToggleTrayMenu(BOOL bEnable)
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
    m_trayMenu.GetSubMenu(0)->EnableMenuItem(ID_TRAY_EMPTYCLIPBOARD, lFlags);
}

// Starts the hog video window. 
// Verifies if the setting is enabled before doing any work.
void screendumpDlg::StartHog()
{
	// Check if its already running. If it is, then ignore the hog request.
	if(!m_Hog.IsHogging())
	{
		GlobalSettings gs;
		if (gs.bEnableHog)
		{
			CString strFileName( m_runningPath + _T("bs.dont.delete.me"));
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
				MessageBox(tmpErrStr, _T("screendump->InitDialog()->EnablingHog"), MB_OK|MB_ICONERROR);
				gs.bEnableHog = FALSE;
				gs.WriteSettings();
			}
		}
	}
}

// Stops the hog window. 
void screendumpDlg::StopHog()
{
	// Only stop it if it was running.
	if(m_Hog.IsHogging())
	{
	    m_Hog.UnHog();
	}
}

void screendumpDlg::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
    lpwndpos->flags &= ~SWP_SHOWWINDOW;
    CDialog::OnWindowPosChanging(lpwndpos);
}

void screendumpDlg::RequestCapture(CaptureType ct)
{
    GlobalSettings gs;
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
            MessageBox(errMsg, _T("screendump->RequestCapture"), MB_ICONERROR|MB_OK);
        }
    }
    if(fName.GetLength() > 0)
    {
        switch (ct)
        {
            case ct_Screen:
                wc->CaptureScreen(fName);
                break;
            case ct_Window:
                wc->CaptureWindow(fName);
                break;
                
        }     
    }
}

BOOL screendumpDlg::OnQueryEndSession()
{
	OnClose();
	return TRUE;
}

void screendumpDlg::OnClose()
{
	DoUnregisterHotKeys();
	ShellIcon_Terminate(); 
	CDialog::OnClose();
	EndDialog(1);
}

void screendumpDlg::OnTrayEmptyclipboard()
{
    if(OpenClipboard())
    {
        EmptyClipboard();
        CloseClipboard();
    }
}
