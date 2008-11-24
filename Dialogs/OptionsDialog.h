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
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedRadiobmp();
	afx_msg void OnBnClickedRadiopng();
	afx_msg void OnBnClickedRadiojpeg();
	afx_msg void OnBnClickedBtnbrowse();
    afx_msg void OnEnChangeTxtdestination();
	afx_msg void OnEnChangeTxtquality();	
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
	CStatic c_lblErrMsg;
    CButton c_chkWantClipboard;
};
#endif
