#pragma once

#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

// OptionsDialog dialog
#include "resource.h"

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


	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedRadiobmp();
	afx_msg void OnBnClickedRadiopng();
	afx_msg void OnBnClickedRadiojpeg();
	afx_msg void OnBnClickedBtnbrowse();
	virtual BOOL OnInitDialog();
};
#endif
