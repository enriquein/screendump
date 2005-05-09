// bScreenDumped2Dlg.h : header file
//

#pragma once
#include "HogVideo.h"
#include "globalatom.h"
#include "gdiplus.h" 
using namespace Gdiplus;

// CbScreenDumped2Dlg dialog
class CbScreenDumped2Dlg : public CDialog
{
// Construction
public:
	CbScreenDumped2Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_BSCREENDUMPED2_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	void ShellIcon_Initialize();
	void ShellIcon_Terminate();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	void DoRegisterHotKeys();
	void DoUnregisterHotKeys();
	void DoCleanup();
	CGlobalAtom* m_Atom;
	CGlobalAtom* m_AtomAlt;
	GdiplusStartupInput m_gdiPlusStatupInput;
	ULONG_PTR m_gdiPlusToken; 
	CHogVideo m_Hog;

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg LRESULT ShellIconCallback(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT ProcessHotKey(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};
