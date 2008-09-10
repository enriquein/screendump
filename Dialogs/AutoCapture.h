#pragma once
#ifndef AUTOCAPTURE_H
#define AUTOCAPTURE_H
#include "afxwin.h"

class CAutoCapture : public CDialog
{
	DECLARE_DYNAMIC(CAutoCapture)

public:
	CAutoCapture(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAutoCapture();
    static UINT UWM_TIMER_AC;

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