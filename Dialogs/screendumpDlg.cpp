#pragma once
#include "stdafx.h"
#include "..\Classes\HogVideo.h"
#include ".\screendumpDlg.h"
#include "..\Classes\WindowCapture.h"
#include "..\Classes\GlobalSettings.h"
#include ".\AboutDialog.h"
#include ".\AutoCapture.h"
#include "..\Classes\file_ver.h"
#include "..\Classes\ErrorString.h"
#include "..\Classes\FileOperations.h"
#include "..\Classes\AeroCheck.h"
#include ".\OptionsDialog.h"

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
        GlobalSettings gs;
        wc = new WindowCapture(gs);
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
	ON_COMMAND(ID_TRAY_HELP, OnTrayHelpClick)
    ON_COMMAND(ID_TRAY_EMPTYCLIPBOARD, OnTrayEmptyclipboard)	
    ON_WM_WINDOWPOSCHANGING()
	ON_WM_QUERYENDSESSION()
	ON_WM_CLOSE()
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

// Starts the hog video window. 
// Verifies if the setting is enabled before doing any work.
void screendumpDlg::StartHog()
{
    // We don't want to hog under Aero, no point in it.
    if(CheckAeroIsEnabled())
        return;

	// Check if its already running. If it is, then ignore the hog request.
	if(m_Hog.IsHogging())
	    return;
	    
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

void screendumpDlg::RequestCapture(const CaptureType& ct)
{
    GlobalSettings gs;
    CString autoFileName;   // Autogenerated
    CString SaveAsFileName; // User-defined from the 'Save As' dialog.
    
    // We first generate an autofilename to take the screenie. Regardless of setting.
    autoFileName = gs.getOutputDir() + GenerateAutoFileName();
    if(autoFileName.GetLength() > 0)
    {
        switch (ct)
        {
            case ct_Screen:
                wc->CaptureScreen(autoFileName);
                break;
            case ct_Window:
                wc->CaptureWindow(autoFileName);
                break;
                
        }     
    }
    
    if (gs.bAutoName == false)
    {
        SaveAsFileName = OpenFileDialog();
        if(SaveAsFileName.GetLength() > 0)
        {
            MoveFile(_T("\\\\?\\") + autoFileName, _T("\\\\?\\") + SaveAsFileName);
        }
    }
    
}
