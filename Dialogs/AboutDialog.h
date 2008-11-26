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
    CEdit c_txtBzrID;    
    CString strVersionText;
    CString strBuildDate;
    
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
    afx_msg void OnStnClickedUrlLink();
    afx_msg void OnStnClickedEmailLink();
    afx_msg void OnStnClickedLaunchpadLink();

};

// Inline defs

inline void AboutDialog::OnStnClickedUrlLink()
{
    ShellExecute(NULL, _T("open"), _T("http://www.indiecodelabs.com/"), NULL, NULL, SW_SHOWNORMAL);
}

inline void AboutDialog::OnStnClickedLaunchpadLink()
{
    ShellExecute(NULL, _T("open"), _T("https://bugs.launchpad.net/screendump"), NULL, NULL, SW_SHOWNORMAL);
}

inline void AboutDialog::OnStnClickedEmailLink()
{
    ShellExecute(NULL, _T("open"), _T("mailto:dev@indiecodelabs.com"), NULL, NULL, SW_SHOWNORMAL);
}

#endif
