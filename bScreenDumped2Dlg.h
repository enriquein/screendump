// bScreenDumped2Dlg.h : header file
//

#pragma once


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

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg LRESULT ShellIconCallback(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};
