// bScreenDumped2Dlg.h : header file
//

#pragma once
#ifndef BSCREENDUMPED2DLG_H
#define BSCREENDUMPED2DLG_H

#include "..\Classes\HogVideo.h"
#include "..\Classes\globalatom.h"
#include "..\res\resource.h"
#include "gdiplus.h" 
using namespace Gdiplus;



// CbScreenDumped2Dlg dialog
class CbScreenDumped2Dlg : public CDialog
{
// Construction
public:
	CbScreenDumped2Dlg(CWnd* pParent = NULL);	// standard constructor
	CMenu m_trayMenu;
	CString m_progVersion;
    void ToggleTrayMenu(BOOL bEnable); // Enables/Disables the tray menu items.
    void StartHog(); // Starts the hogger control.
    static UINT UWM_SHELLICON_MSG;

// Dialog Data
	enum { IDD = IDD_BSCREENDUMPED2_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	void ShellIcon_Initialize();
	void ShellIcon_Terminate();
	void DoRegisterHotKeys();
	void DoUnregisterHotKeys();
	void DoCleanup();
	CGlobalAtom* m_Atom;
	CGlobalAtom* m_AtomAlt;
    CHogVideo m_Hog;
	GdiplusStartupInput m_gdiPlusStatupInput;
	ULONG_PTR m_gdiPlusToken; 
	HICON m_hIcon;
    BOOL isVisible; 

	// Tray Handlers
	void OnTrayExitClick();
	void OnTrayAboutClick();
	void OnTrayOptionsClick();
	void OnTrayOpenDest();
	void OnTrayAutoCapture();

	// Message map functions
	virtual BOOL OnInitDialog();
	afx_msg LRESULT ShellIconCallback(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT ProcessHotKey(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
};

#endif
