// AboutDialog.cpp : implementation file
//
#include "stdafx.h"
#include "..\res\resource.h"
#include ".\aboutdialog.h"

UINT CAboutDialog::UWM_REQUESTVERSION = ::RegisterWindowMessage(_T("UWM_REQUESTVERSION-{F9264F49-8BFF-4667-8C00-9B9E8E9D0485}"));

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
    DDX_Control(pDX, IDC_LOGOIMG, c_LogoImg);
}


BEGIN_MESSAGE_MAP(CAboutDialog, CDialog)
    ON_BN_CLICKED(IDOK, &CAboutDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CAboutDialog message handlers

BOOL CAboutDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	CString strVersionText;
	LRESULT lres;
		
	// Get version number from our main dialog.
    lres = ::SendMessage(GetParent()->m_hWnd, UWM_REQUESTVERSION, 0, 0);
    strVersionText = *(CString*)lres;
    strVersionText = _T("About screendump ") + strVersionText + CString(_T(" built on: ")) + CString(_T(__DATE__)) + CString(_T(__TIME__));
    SetWindowText(strVersionText);
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CAboutDialog::OnBnClickedOk()
{
    // TODO: Add your control notification handler code here
    OnOK();
}
