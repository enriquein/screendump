// bScreenDumped2Dlg.h : header file
//

#pragma once
#include "globalatom.h"

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

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg LRESULT ShellIconCallback(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT ProcessHotKey(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};
