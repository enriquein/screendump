// bScreenDumped2Dlg.h : header file
//

#pragma once
#ifndef BSCREENDUMPED2DLG_H
#define BSCREENDUMPED2DLG_H

#include "..\Classes\HogVideo.h"
#include "..\Classes\globalatom.h"
#include "..\res\resource.h"
#include "..\Classes\GlobalSettings.h"
#include "..\Classes\WindowCapture.h"

// CbScreenDumped2Dlg dialog
class CbScreenDumped2Dlg : public CDialog
{
// Construction
public:
	CbScreenDumped2Dlg(CWnd* pParent = NULL);	// standard constructor
    ~CbScreenDumped2Dlg();

    // ShellIcon Callback msg
    static UINT UWM_SHELLICON_MSG;

    // Cross-dialog messages
    static UINT UWM_TOGGLETRAY;
    static UINT UWM_CAPTURESCREEN;
    static UINT UWM_CAPTUREWINDOW;
    static UINT UWM_REQUESTHOG;

    // Dialog Data
	enum { IDD = IDD_BSCREENDUMPED2_DIALOG };
	CString m_progVersion;

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
    void RequestWindowCapture();
    void RequestScreenCapture();

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
    afx_msg LRESULT OnRequestHog(WPARAM wParam, LPARAM lParam);

	// Message map functions
	virtual BOOL OnInitDialog();
	afx_msg LRESULT ShellIconCallback(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT ProcessHotKey(WPARAM wParam, LPARAM lParam);
    afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
	DECLARE_MESSAGE_MAP()
};

#endif
