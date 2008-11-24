// screendumpDlg.h : header file
//

#pragma once
#ifndef SCREENDUMPDLG_H
#define SCREENDUMPDLG_H

#include "..\Classes\HogVideo.h"
#include "..\Classes\globalatom.h"
#include "..\res\resource.h"
#include "..\Classes\GlobalSettings.h"
#include "..\Classes\WindowCapture.h"

// CscreendumpDlg dialog
class CscreendumpDlg : public CDialog
{
// Construction
public:
	CscreendumpDlg(CWnd* pParent = NULL);	// standard constructor
    ~CscreendumpDlg();

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
	CGlobalAtom* m_Atom;
	CGlobalAtom* m_AtomAlt;
    CHogVideo m_Hog;
	HICON m_hIcon;
	CMenu m_trayMenu;

	virtual void DoDataExchange(CDataExchange* pDX);	    
    void ToggleTrayMenu(BOOL bEnable); 
    void StartHog(); 
    void StopHog();
	void ShellIcon_Initialize();
	void ShellIcon_Terminate();
	void DoRegisterHotKeys();
	void DoUnregisterHotKeys();
    void RequestCapture(CaptureType ct);
    void RefreshSettings();

	// Tray Handlers
	void OnTrayExitClick();
	void OnTrayAboutClick();
	void OnTrayOptionsClick();
	void OnTrayOpenDest();
	void OnTrayAutoCapture();

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

#endif
