#pragma once

#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

// OptionsDialog dialog
#include "..\res\resource.h"
#include "afxwin.h"

class OptionsDialog : public CDialog
{
	DECLARE_DYNAMIC(OptionsDialog)

public:
	OptionsDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~OptionsDialog();

// Dialog Data
	enum { IDD = IDD_SETTINGSDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    void updateControls();
    void initializeControls();
	void SendHogMessage(BOOL bEnableHog);
	static UINT UWM_REQUESTHOG;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedRadiobmp();
	afx_msg void OnBnClickedRadiopng();
	afx_msg void OnBnClickedRadiojpeg();
	afx_msg void OnBnClickedBtnbrowse();
	virtual BOOL OnInitDialog();

private:
    CButton c_OK;
    CEdit c_Destination;
    CEdit c_JpegQuality;
    CButton c_optBmp;
    CButton c_optJpeg;
    CButton c_optPng;
    CButton c_chkAutoName;
    CButton c_chkHog;
    BOOL initialized;
public:
	afx_msg void OnEnChangeTxtquality();
private:
	CStatic c_lblErrMsg;
public:
    afx_msg void OnEnChangeTxtdestination();
};
#endif
