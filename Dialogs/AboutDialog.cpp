// AboutDialog.cpp : implementation file
//
#include "stdafx.h"
#include "..\res\resource.h"
#include ".\bScreenDumped2Dlg.h"
#include ".\aboutdialog.h"


// CAboutDialog dialog

IMPLEMENT_DYNAMIC(CAboutDialog, CDialog)
CAboutDialog::CAboutDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CAboutDialog::IDD, pParent)
{
}

CAboutDialog::~CAboutDialog()
{
}

void CAboutDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAboutDialog, CDialog)
END_MESSAGE_MAP()


// CAboutDialog message handlers

BOOL CAboutDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	CString strVersionText;
	// This is pretty ugly, I suppose. Should reconsider rethinking this through.
	CbScreenDumped2Dlg* mainDlg;
	mainDlg = (CbScreenDumped2Dlg*)AfxGetApp()->m_pMainWnd;
    strVersionText = _T("About bScreenDumped ") + mainDlg->m_progVersion;
    SetWindowText(strVersionText);
	return TRUE;  // return TRUE unless you set the focus to a control
}
