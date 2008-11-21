// AboutDialog.cpp : implementation file
//
#include "stdafx.h"
#include "..\res\resource.h"
#include ".\aboutdialog.h"
#include "..\Classes\Hyperlinks.h"

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
    DDX_Control(pDX, IDC_BUILDTEXT, c_BuildText);
}


BEGIN_MESSAGE_MAP(CAboutDialog, CDialog)
    ON_STN_CLICKED(IDC_URLLINK, &CAboutDialog::OnStnClickedUrllink)
END_MESSAGE_MAP()


// CAboutDialog message handlers

BOOL CAboutDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	LRESULT lres;
	
	// Convert our Static control into a Hyperlink
	ConvertStaticToHyperlink(GetSafeHwnd(), IDC_URLLINK);
	
	// Get version number from our main dialog.
    lres = ::SendMessage(GetParent()->m_hWnd, UWM_REQUESTVERSION, 0, 0);
    strVersionText = *(CString*)lres;
    SetWindowText( _T("About screendump ") + strVersionText );
    
    // Set build date
    strBuildDate = CString(_T(__DATE__)) + CString(_T(" ")) + CString(_T(__TIME__));
    
    c_BuildText.SetWindowText(_T("You are currently using screendump version ") + strVersionText + _T(". Built on ") + strBuildDate + _T("."));
    
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CAboutDialog::OnStnClickedUrllink()
{
    ShellExecute(NULL, _T("open"), _T("http://www.indiecodelabs.com/"), NULL, NULL, SW_SHOWNORMAL);
}
