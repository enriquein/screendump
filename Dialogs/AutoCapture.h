#pragma once


// CAutoCapture dialog

#define TIMERAUTOCAPTURE (WM_USER+102)

class CAutoCapture : public CDialog
{
	DECLARE_DYNAMIC(CAutoCapture)

public:
	CAutoCapture(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAutoCapture();


// Dialog Data
	enum { IDD = IDD_AUTOTIMER };

protected:
	BOOL bCatchForeground;
	void EnableControls(BOOL bEnable);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg LRESULT OnTimer(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
protected:
	virtual void OnCancel();
public:
	afx_msg void OnClose();
};
