#pragma once

#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H
#include "afxwin.h"
// AboutDialog dialog

class AboutDialog : public CDialog
{
	DECLARE_DYNAMIC(AboutDialog)

public:
	AboutDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~AboutDialog();

    static UINT UWM_REQUESTVERSION;

// Dialog Data
	enum { IDD = IDD_ABOUTDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    CStatic c_LogoImg;    
    CStatic c_BuildText; 
    CString strVersionText;
    CString strBuildDate;
    
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
    afx_msg void OnStnClickedUrllink();
};

// Inline defs

inline void AboutDialog::OnStnClickedUrllink()
{
    ShellExecute(NULL, _T("open"), _T("http://www.indiecodelabs.com/"), NULL, NULL, SW_SHOWNORMAL);
}

#endif
