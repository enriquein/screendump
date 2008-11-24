#pragma once

#ifndef CABOUTDIALOG_H
#define CABOUTDIALOG_H
#include "afxwin.h"
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
    CStatic c_LogoImg;    
    CString strVersionText;
    CString strBuildDate;
    
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
    afx_msg void OnStnClickedUrllink();
protected:
    CStatic c_BuildText;
};
#endif
