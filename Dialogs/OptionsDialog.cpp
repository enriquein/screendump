#include "stdafx.h"
#include <io.h>
#include <direct.h>
#include ".\OptionsDialog.h"
#include "..\Classes\GlobalSettings.h"
#include "..\Helpers\Helpers.h"

IMPLEMENT_DYNAMIC(OptionsDialog, CDialog)
OptionsDialog::OptionsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(OptionsDialog::IDD, pParent)
{
    initialized = false;
}

OptionsDialog::~OptionsDialog()
{
}

void OptionsDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDOK, c_OK);
    DDX_Control(pDX, IDC_TXTDESTINATION, c_Destination);
    DDX_Control(pDX, IDC_TXTQUALITY, c_JpegQuality);
    DDX_Control(pDX, IDC_RADIOBMP, c_optBmp);
    DDX_Control(pDX, IDC_RADIOJPEG, c_optJpeg);
    DDX_Control(pDX, IDC_RADIOPNG, c_optPng);
    DDX_Control(pDX, IDC_CHKAUTONAME, c_chkAutoName);
    DDX_Control(pDX, IDC_CHKHOG, c_chkHog);
}


BEGIN_MESSAGE_MAP(OptionsDialog, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_RADIOBMP, OnBnClickedRadiobmp)
	ON_BN_CLICKED(IDC_RADIOPNG, OnBnClickedRadiopng)
	ON_BN_CLICKED(IDC_RADIOJPEG, OnBnClickedRadiojpeg)
	ON_BN_CLICKED(IDC_BTNBROWSE, OnBnClickedBtnbrowse)
END_MESSAGE_MAP()


// OptionsDialog message handlers
void OptionsDialog::OnBnClickedOk()
{
	// Validate!
	// Quality TextBox
	CString strVal;
    int val = 0;
    if(c_optJpeg.GetCheck() == BST_CHECKED)
	{
        c_JpegQuality.GetWindowText(strVal);
		val = _ttoi(strVal);
		if( (val > 100) || (val <= 0) )  
		{
			MessageBox(_T("Please enter a number between 1 and 100."), _T("bScreenDumped->Options"), MB_OK|MB_ICONERROR);
			c_JpegQuality.SetFocus();
			c_JpegQuality.SetSel(0,4, true);
			return;
		}
	}

	// Path TextBox
	c_Destination.GetWindowText(strVal);
	if(_taccess(strVal, 0) == -1)
	{
		if( MessageBox(_T("The selected directory does not exist. Do you wish to create it?"), _T("bScreenDumped->Options"), MB_YESNO|MB_ICONQUESTION) == IDYES )
		{
			if(_tmkdir(strVal) != 0)
			{
				MessageBox(_T("Unable to create directory, please make sure the path is correct."), _T("bScreenDumped->Options"), MB_OK|MB_ICONERROR);
				c_Destination.SetFocus();
				c_Destination.SetSel(0, c_Destination.GetWindowTextLength(), true);
				return;
			}
		}
		else
		{
			MessageBox(_T("You chose to not create the directory. Please choose a valid destination before proceeding."), _T("bScreenDumped->Options"), MB_OK|MB_ICONINFORMATION);
			c_Destination.SetFocus();
			c_Destination.SetSel(0, c_Destination.GetWindowTextLength(), true);
			return;
		}
	}

	// Save Settings
	CGlobalSettings gs;
	gs.ReadSettings();

    gs.bAutoName = (c_chkAutoName.GetCheck() == BST_CHECKED) ? true : false;
	gs.bEnableHog = (c_chkHog.GetCheck() == BST_CHECKED) ? true : false;

    if( c_optBmp.GetCheck() == BST_CHECKED )
    {
		gs.sEnc = sEncBMP;
    }

    if( c_optJpeg.GetCheck() == BST_CHECKED )
    {	
        gs.sEnc = sEncJPEG;
		c_JpegQuality.GetWindowText(strVal);
		gs.lJpgQuality = _ttol(strVal);
    }

    if( c_optPng.GetCheck() == BST_CHECKED )
    {
		gs.sEnc = sEncPNG;
    }
    c_Destination.GetWindowText(gs.szOutputDir);
	gs.WriteSettings();
	
    // Doesn't matter if this gets called since settings are saved/reloaded.
	OnCancel();
}

void OptionsDialog::OnBnClickedCancel()
{
	OnCancel();
}

void OptionsDialog::OnBnClickedRadiobmp()
{
	updateControls(); 
}

void OptionsDialog::OnBnClickedRadiopng()
{
	updateControls(); 
}

void OptionsDialog::OnBnClickedRadiojpeg()
{
	updateControls(); 
}

void OptionsDialog::OnBnClickedBtnbrowse()
{
	CFileDialog* cfDlg = new CFileDialog(TRUE, NULL, _T("DO NOT DELETE THIS TEXT"), OFN_HIDEREADONLY|OFN_EXPLORER, _T("Directories|*.12ewdfvrfter||"), this);
	if(cfDlg->DoModal() == IDOK)
	{
		CString csPath = cfDlg->GetPathName();
		csPath = csPath.Left( csPath.ReverseFind(_T('\\')) );
        c_Destination.SetWindowText(csPath);
	}
	delete cfDlg;
}

BOOL OptionsDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
    // TODO: well more of a reminder. check comment below
	//ShowWindow(SW_NORMAL);

    initialized = true;
    initializeControls();
    updateControls();
	return TRUE; 
}

void OptionsDialog::initializeControls()
{
	// Load Settings
	CGlobalSettings gs;
	gs.ReadSettings();

	// Initialize Controls
	switch(gs.sEnc)
	{
	case sEncBMP:
        c_optBmp.SetCheck(BST_CHECKED);
		OnBnClickedRadiobmp();
		break;

	case sEncJPEG:
        c_optJpeg.SetCheck(BST_CHECKED);
		OnBnClickedRadiojpeg();
		break;

	case sEncPNG:
        c_optPng.SetCheck(BST_CHECKED);
		OnBnClickedRadiopng();
		break;

	default:
		MessageBox(_T("Encoder Value not found. This is a very unusual error indeed."), _T("bScreenDumped->OptionsDialog()->initializeControls"), MB_OK | MB_ICONWARNING);
		break;
	}

    CString tmpStr;
	tmpStr.Format(_T("%d"), gs.lJpgQuality);
    c_JpegQuality.SetWindowText(tmpStr);	
	c_JpegQuality.SetLimitText(3);
    
	if(gs.bAutoName)
	{
		c_chkAutoName.SetCheck(BST_CHECKED);
	}
	else
	{
		c_chkAutoName.SetCheck(BST_UNCHECKED);
	}

	if(gs.bEnableHog)
	{
        c_chkHog.SetCheck(BST_CHECKED);
	}
	else
	{
		c_chkHog.SetCheck(BST_UNCHECKED);
	}
    c_Destination.SetWindowText(gs.szOutputDir);
}

// All the control validation logic goes here
void OptionsDialog::updateControls()
{
    // a little sanity checking in case the controls haven't been initialized yet.
    if(!initialized)
    {
        return;
    }

    bool enable;
    CString tmpStr;
    int iDestLen = 0, iJpgQualLen = 0;
    c_Destination.GetWindowText(tmpStr);
    tmpStr = tmpStr.Trim();
    iDestLen = tmpStr.GetLength();
    if( c_optJpeg.GetCheck() == BST_CHECKED )
    {
        c_JpegQuality.GetWindowText(tmpStr);
        tmpStr = tmpStr.Trim();
        iJpgQualLen = tmpStr.GetLength();
    }
    enable = ( (iDestLen > 0) && ( (c_optBmp.GetCheck() == BST_CHECKED) || (c_optJpeg.GetCheck() == BST_CHECKED) || (c_optPng.GetCheck() == BST_CHECKED) ) );
    if( (c_optJpeg.GetCheck() == BST_CHECKED) && (iJpgQualLen == 0) )
    {
        enable = false;
    }
    c_JpegQuality.EnableWindow(c_optJpeg.GetCheck());
    c_OK.EnableWindow(enable);
}
