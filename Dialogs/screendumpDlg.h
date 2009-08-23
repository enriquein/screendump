#pragma once
#include "..\Classes\HogVideo.h"
#include "..\Classes\globalatom.h"
#include "..\res\resource.h"
#include "..\Classes\GlobalSettings.h"
#include "..\Classes\WindowCapture.h"
#include ".\OptionsDialog.h"
#include ".\AboutDialog.h"
#include ".\AutoCapture.h"

// screendumpDlg dialog
class screendumpDlg : public CDialog
{
// Construction
public:
	screendumpDlg(CWnd* pParent = NULL);	// standard constructor
    ~screendumpDlg();

    // ShellIcon Callback msg
    static UINT UWM_SHELLICON_MSG;

    // Cross-dialog messages
    static UINT UWM_TOGGLETRAY;
    static UINT UWM_CAPTURESCREEN;
    static UINT UWM_CAPTUREWINDOW;
    static UINT UWM_REQUESTVERSION;

    // Dialog Data
	enum { IDD = IDD_screendump_DIALOG };
	CString m_progVersion; // Program's version number and revision. Used for our tooltip and about dialog.
	CString m_runningPath; // Path where our program is located. 

protected:
    // Data Members
    WindowCapture* wc;
	GlobalAtom* m_Atom;
	GlobalAtom* m_AtomAlt;
    HogVideo m_Hog;
	HICON m_hIcon;
	CMenu m_trayMenu;

	virtual void DoDataExchange(CDataExchange* pDX);	    
    void ToggleTrayMenu(const BOOL& bEnable); 
    void StartHog(); 
    void StopHog();
	void ShellIcon_Initialize();
	void ShellIcon_Terminate();
	void DoRegisterHotKeys();
	void DoUnregisterHotKeys();
    void RequestCapture(const CaptureType& ct);
    void RefreshSettings();

	// Tray Handlers
	void OnTrayExitClick();
	void OnTrayAboutClick();
	void OnTrayOptionsClick();
	void OnTrayOpenDest();
	void OnTrayAutoCapture();
	void OnTrayHelpClick();

    // CrossDialog Message Handlers
    afx_msg LRESULT OnToggleTrayMsg(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnCaptureScreenMsg(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnCaptureWindowMsg(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnRequestVersion(WPARAM wParam, LPARAM lParam);

	// Message map functions
	virtual BOOL OnInitDialog();
	afx_msg LRESULT ShellIconCallback(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT ProcessHotKey(WPARAM wParam, LPARAM lParam);
    afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
	afx_msg BOOL OnQueryEndSession();
	afx_msg void OnClose();
    afx_msg void OnTrayEmptyclipboard();
	DECLARE_MESSAGE_MAP()
};

// inline defs
inline void screendumpDlg::ShellIcon_Terminate()
{
	NOTIFYICONDATA ni;
	ni.cbSize = NOTIFYICONDATA_V1_SIZE; 
	ni.hWnd = m_hWnd; 
	ni.uID = 1;
	Shell_NotifyIcon(NIM_DELETE, &ni);
}

inline void screendumpDlg::OnTrayExitClick()
{
	OnClose();
}

inline void screendumpDlg::OnTrayAboutClick()
{
	AboutDialog cAbt((CWnd*)this);
	ToggleTrayMenu(FALSE);
	cAbt.DoModal();
	ToggleTrayMenu(TRUE);
}

inline void screendumpDlg::OnTrayOpenDest()
{
	GlobalSettings gs;
	ShellExecute(m_hWnd, _T("open"), gs.getOutputDir(), NULL, NULL, SW_SHOWNORMAL);
}

inline void screendumpDlg::OnTrayOptionsClick()
{
	OptionsDialog oDl((CWnd*)this);
	ToggleTrayMenu(FALSE);
	oDl.DoModal();
    RefreshSettings();
	ToggleTrayMenu(TRUE);
}

inline void screendumpDlg::OnTrayAutoCapture()
{
	AutoCapture cAC((CWnd*)this);
	ToggleTrayMenu(FALSE);
	cAC.DoModal();
	ToggleTrayMenu(TRUE);
}

// Allows other dialogs to request Screen Captures.
inline LRESULT screendumpDlg::OnCaptureScreenMsg(WPARAM wParam, LPARAM lParam)
{
    RequestCapture(ct_Screen);
    return 0;
}

// Allows other dialogs to request Window Captures 
inline LRESULT screendumpDlg::OnCaptureWindowMsg(WPARAM wParam, LPARAM lParam)
{
    RequestCapture(ct_Window);
    return 0;
}

inline void screendumpDlg::RefreshSettings()
{
    GlobalSettings gs;
    wc->UpdateSettings(gs);
    StartHog();
}

// (BOOL)wParam tells us if we want to enable/disable the tray. TRUE = enable, FALSE = disable.
inline LRESULT screendumpDlg::OnToggleTrayMsg(WPARAM wParam, LPARAM lParam)
{
    if(wParam != NULL)
    {
        ToggleTrayMenu((BOOL)wParam);
    }
    return 0;
}

// (CString *)(LRESULT) returns a pointer to a CString object that contains our version number.
inline LRESULT screendumpDlg::OnRequestVersion(WPARAM wParam, LPARAM lParam)
{
    return lParam = (LRESULT) &m_progVersion;
}

inline void screendumpDlg::DoRegisterHotKeys()
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

inline void screendumpDlg::DoUnregisterHotKeys()
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

inline LRESULT screendumpDlg::ProcessHotKey(WPARAM wParam, LPARAM lParam)
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

inline void screendumpDlg::ToggleTrayMenu(const BOOL& bEnable)
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
    m_trayMenu.GetSubMenu(0)->EnableMenuItem(ID_TRAY_HELP, lFlags);
}

// Stops the hog window. 
inline void screendumpDlg::StopHog()
{
	// Only stop it if it was running.
	if(m_Hog.IsHogging())
	{
	    m_Hog.UnHog();
	}
}

inline void screendumpDlg::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
    lpwndpos->flags &= ~SWP_SHOWWINDOW;
    CDialog::OnWindowPosChanging(lpwndpos);
}


inline BOOL screendumpDlg::OnQueryEndSession()
{
	OnClose();
	return TRUE;
}

inline void screendumpDlg::OnClose()
{
	DoUnregisterHotKeys();
	ShellIcon_Terminate(); 
	CDialog::OnClose();
	EndDialog(1);
}

inline void screendumpDlg::OnTrayEmptyclipboard()
{
    if(OpenClipboard())
    {
        EmptyClipboard();
        CloseClipboard();
    }
}

inline void screendumpDlg::OnTrayHelpClick()
{
    ShellExecute(m_hWnd, _T("open"), m_runningPath + _T("\\HelpManual.chm"), NULL, NULL, SW_SHOWNORMAL);
}
