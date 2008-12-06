// AboutDialog.cpp : implementation file
//
#include "stdafx.h"
#include "..\res\resource.h"
#include ".\aboutdialog.h"
#include "..\Classes\Hyperlinks.h"

UINT AboutDialog::UWM_REQUESTVERSION = ::RegisterWindowMessage(_T("UWM_REQUESTVERSION-{F9264F49-8BFF-4667-8C00-9B9E8E9D0485}"));
#define BZR_REV_ID _T("dev@indiecodelabs.com-20081206031916-yavefup9hcrtuoqp")

// AboutDialog dialog

IMPLEMENT_DYNAMIC(AboutDialog, CDialog)
AboutDialog::AboutDialog(CWnd* pParent /*=NULL*/)
	: CDialog(AboutDialog::IDD, pParent)
{
}

AboutDialog::~AboutDialog()
{
}

void AboutDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LOGOIMG, c_LogoImg);
    DDX_Control(pDX, IDC_BUILDTEXT, c_BuildText);
    DDX_Control(pDX, IDC_TXTBZRID, c_txtBzrID);
}


BEGIN_MESSAGE_MAP(AboutDialog, CDialog)
    ON_STN_CLICKED(IDC_URLLINK, &AboutDialog::OnStnClickedUrlLink)
    ON_STN_CLICKED(IDC_LAUNCHPADURL, &AboutDialog::OnStnClickedLaunchpadLink)
    ON_STN_CLICKED(IDC_EMAIL, &AboutDialog::OnStnClickedEmailLink)
END_MESSAGE_MAP()


// AboutDialog message handlers

BOOL AboutDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	LRESULT lres;
	
	// Convert our Static control into a Hyperlink
	ConvertStaticToHyperlink(GetSafeHwnd(), IDC_URLLINK);
	ConvertStaticToHyperlink(GetSafeHwnd(), IDC_LAUNCHPADURL);
	ConvertStaticToHyperlink(GetSafeHwnd(), IDC_EMAIL);
	
	// Get version number from our main dialog.
    lres = ::SendMessage(GetParent()->m_hWnd, UWM_REQUESTVERSION, 0, 0);
    strVersionText = *(CString*)lres;
    SetWindowText( _T("About screendump ") + strVersionText );
    
    // Set build date
    strBuildDate = CString(_T(__DATE__)) + CString(_T(" ")) + CString(_T(__TIME__));
    
    c_BuildText.SetWindowText(_T("You are using screendump version ") + strVersionText + _T(". Built on ") + strBuildDate + _T(". For support purposes, don't forget to copy and paste the following ID into your email or bug report:"));
    
    c_txtBzrID.SetWindowText(BZR_REV_ID);
    
	return TRUE;  // return TRUE unless you set the focus to a control
}
