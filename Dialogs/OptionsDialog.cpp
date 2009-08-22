#pragma once
#include "stdafx.h"
#include <io.h>
#include <direct.h>
#include ".\OptionsDialog.h"
#include "..\Classes\GlobalSettings.h"
#include "..\Classes\AeroCheck.h"

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
    DDX_Control(pDX, IDC_ERRORMSG, c_lblErrMsg);
    DDX_Control(pDX, IDC_WANTCLIPBOARD, c_chkWantClipboard);
    DDX_Control(pDX, IDC_WANTCURSOR, c_WantCursor);
}


BEGIN_MESSAGE_MAP(OptionsDialog, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_RADIOBMP, OnBnClickedRadiobmp)
	ON_BN_CLICKED(IDC_RADIOPNG, OnBnClickedRadiopng)
	ON_BN_CLICKED(IDC_RADIOJPEG, OnBnClickedRadiojpeg)
	ON_BN_CLICKED(IDC_BTNBROWSE, OnBnClickedBtnbrowse)
	ON_EN_CHANGE(IDC_TXTQUALITY, OnEnChangeTxtquality)
    ON_EN_CHANGE(IDC_TXTDESTINATION, OnEnChangeTxtdestination)
    ON_BN_CLICKED(IDC_CHKAUTONAME, OnBnClickedChkautoname)
END_MESSAGE_MAP()


// OptionsDialog message handlers
void OptionsDialog::OnBnClickedOk()
{
	// We no longer validate here since all of the validation has moved to updateControls()
	// Quality TextBox
	CString strVal;

	// Ok, so not all of the validation was practical at updateControls. I want to make sure I get a valid path.
	c_Destination.GetWindowText(strVal);
	if(_taccess(strVal, 0) == -1)
	{
		if( MessageBox(_T("The selected directory does not exist: ") + strVal + _T("\nDo you wish to create it?"), _T("screendump->Options"), MB_YESNO|MB_ICONQUESTION) == IDYES )
		{
			if(_tmkdir(strVal) != 0)
			{
				MessageBox(_T("Unable to create directory: ") + strVal + _T("\nPlease make sure the path is correct and not read-only."), _T("screendump->Options"), MB_OK|MB_ICONERROR);
				c_Destination.SetFocus();
				c_Destination.SetSel(0, c_Destination.GetWindowTextLength(), TRUE);
				return;
			}
            else
            {
                MessageBox(_T("The selected directory: ") + strVal + _T(" was successfully created."), _T("screendump->Options"), MB_OK|MB_ICONINFORMATION);
            }
		}
		else
		{
			MessageBox(_T("You chose to not create the directory. Please choose a valid destination before proceeding."), _T("screendump->Options"), MB_OK|MB_ICONINFORMATION);
			c_Destination.SetFocus();
			c_Destination.SetSel(0, c_Destination.GetWindowTextLength(), TRUE);
			return;
		}
	}

	// Save Settings
	GlobalSettings gs;
    gs.bAutoName = (c_chkAutoName.GetCheck() == BST_CHECKED) ? TRUE : FALSE;
	gs.bEnableHog = (c_chkHog.GetCheck() == BST_CHECKED) ? TRUE : FALSE;
    gs.bWantClipboard = (c_chkWantClipboard.GetCheck() == BST_CHECKED) ? TRUE : FALSE;
    gs.bWantCursor = (c_WantCursor.GetCheck() == BST_CHECKED) ? TRUE : FALSE;

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
	c_Destination.GetWindowText(strVal);
	gs.setOutputDir(strVal);
	gs.WriteSettings();

    // Contrary to what intuition will tell you, it doesn't matter if OnCancel gets called since settings were saved.
	OnCancel();
}

BOOL OptionsDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
    initialized = true;
    initializeControls();
    updateControls();
	return TRUE; 
}

void OptionsDialog::initializeControls()
{
	// Load Settings
	GlobalSettings gs;

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
		MessageBox(_T("Encoder Value not found. This is a very unusual error indeed."), _T("screendump->OptionsDialog()->initializeControls"), MB_OK | MB_ICONWARNING);
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

    // Since there's no point to hog while Aero is on, let's not even leave the choice.
    // I'm hoping it's clear to the user that under Aero they have full support for 
    // screenshots of video windows.
    if(CheckAeroIsEnabled())
    {
        c_chkHog.SetCheck(BST_CHECKED);
        c_chkHog.EnableWindow(FALSE);    
    }
    else
    {
	    if(gs.bEnableHog)
	    {
            c_chkHog.SetCheck(BST_CHECKED);
	    }
	    else
	    {
		    c_chkHog.SetCheck(BST_UNCHECKED);
	    }
    }

	if(gs.bWantClipboard)
	{
        c_chkWantClipboard.SetCheck(BST_CHECKED);
	}
	else
	{
		c_chkWantClipboard.SetCheck(BST_UNCHECKED);
	}
	
	if(gs.bWantCursor)
	{
	    c_WantCursor.SetCheck(BST_CHECKED);
	}
	else
	{
	    c_WantCursor.SetCheck(BST_UNCHECKED);
	}
	
    c_Destination.SetWindowText( gs.getOutputDir() );
}

// All the control validation logic goes here.
// The idea is to not let the Ok button become enabled unless all
// validation turns out Ok.
void OptionsDialog::updateControls()
{
    // a little sanity checking in case the controls haven't been initialized yet.
    if(!initialized)
    {
        return;
    }

    BOOL enable;
    CString tmpStr;
    int iDestLen = 0, iJpgQualLen = 0;
    c_Destination.GetWindowText(tmpStr);
    tmpStr = tmpStr.Trim();
    iDestLen = tmpStr.GetLength();

	if( (iDestLen == 0) && (c_chkAutoName.GetCheck() == BST_CHECKED) )
	{
		c_lblErrMsg.SetWindowTextW(_T("You must specify a valid output directory if you wish to use the Autonaming feature."));
		enable = FALSE;
	}
    if ( !( (c_optBmp.GetCheck() == BST_CHECKED) || (c_optJpeg.GetCheck() == BST_CHECKED) || (c_optPng.GetCheck() == BST_CHECKED) ) )
	{
		c_lblErrMsg.SetWindowTextW(_T("You must choose one of the image formats "));
		enable = FALSE;
	}

    if( c_optJpeg.GetCheck() == BST_CHECKED )
    {
        int iQuality;
        c_JpegQuality.GetWindowText(tmpStr);
        tmpStr = tmpStr.Trim();
        if(tmpStr.GetLength() == 0)
        {
            tmpStr = "0";
        }
        iQuality = _ttoi(tmpStr);
    	
	    if( (iQuality <= 0) || (iQuality > 100) )
        {
		    c_lblErrMsg.SetWindowTextW(_T("The quality value you entered is invalid. Please enter a number between 1 and 100."));
            enable = FALSE;
        }
    }
	c_lblErrMsg.ShowWindow(enable ? SW_HIDE : SW_SHOW);
    c_JpegQuality.EnableWindow(c_optJpeg.GetCheck());
    c_OK.EnableWindow(enable);
}

