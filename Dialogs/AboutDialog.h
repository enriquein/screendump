#pragma once
#include "afxwin.h"

#ifndef CABOUTDIALOG_H
#define CABOUTDIALOG_H
// CAboutDialog dialog

class CAboutDialog : public CDialog
{
	DECLARE_DYNAMIC(CAboutDialog)

public:
	CAboutDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAboutDialog();

    static UINT UWM_REQUESTVERSION;

// Dialog Data
	enum { IDD = IDD_ABOUTDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
    CStatic c_LogoImg;
    afx_msg void OnBnClickedOk();
};
#endif
