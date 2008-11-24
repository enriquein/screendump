#pragma once
#ifndef AUTOCAPTURE_H
#define AUTOCAPTURE_H
#include "afxwin.h"

class AutoCapture : public CDialog
{
	DECLARE_DYNAMIC(AutoCapture)

public:
	AutoCapture(CWnd* pParent = NULL);   // standard constructor
	virtual ~AutoCapture();
    static UINT UWM_TIMER_AC;
    static UINT UWM_CAPTURESCREEN;
    static UINT UWM_CAPTUREWINDOW;

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
private:
    CEdit c_txtACDelay;
    CButton c_optSeconds;
    CButton c_optMilliseconds;
    CButton c_optFullScreen;
    CButton c_optForeground;
    CButton c_btnStart;
};
#endif