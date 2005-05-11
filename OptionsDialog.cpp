// OptionsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "afxdlgs.h"
#include "bScreenDumped2.h"
#include "OptionsDialog.h"
#include "GlobalSettings.h"
#include <io.h>
#include <direct.h>

// OptionsDialog dialog

IMPLEMENT_DYNAMIC(OptionsDialog, CDialog)
OptionsDialog::OptionsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(OptionsDialog::IDD, pParent)
{
}

OptionsDialog::~OptionsDialog()
{
}

void OptionsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(OptionsDialog, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_RADIOBMP, OnBnClickedRadiobmp)
	ON_BN_CLICKED(IDC_RADIOTIFF, OnBnClickedRadiotiff)
	ON_BN_CLICKED(IDC_RADIOPNG, OnBnClickedRadiopng)
	ON_BN_CLICKED(IDC_RADIOJPEG, OnBnClickedRadiojpeg)
	ON_BN_CLICKED(IDC_BTNBROWSE, OnBnClickedBtnbrowse)
END_MESSAGE_MAP()


// OptionsDialog message handlers

void OptionsDialog::OnBnClickedOk()
{
	// Validate!
	// Quality TextBox
	CButton* chkJPEG = (CButton*)GetDlgItem(IDC_RADIOJPEG);
	if(chkJPEG->GetCheck() == BST_CHECKED)
	{
		char buffer[4];
		int val;
		CEdit* ceQuality = (CEdit*)GetDlgItem(IDC_TXTQUALITY);
		ceQuality->GetWindowText(buffer, 4);
		val = atoi(buffer);
		if( (val > 100) || (val <= 0) )  
		{
			MessageBox("Please enter a number between 1 and 100.", "bScreenDumped->Options", MB_OK|MB_ICONERROR);
			ceQuality->SetFocus();
			ceQuality->SetSel(0,4, TRUE);
			return;
		}
	}

	// Path TextBox
	CEdit* cePath = (CEdit*)GetDlgItem(IDC_TXTDESTINATION);
	char pathBuffer[_MAX_PATH];
	cePath->GetWindowText(pathBuffer, _MAX_PATH);
	if(_access(pathBuffer, 0) == -1)
	{
		if( MessageBox("The selected directory does not exist. Do you wish to create it?", "bScreenDumped->Options", MB_YESNO|MB_ICONQUESTION) == IDYES )
		{
			if(_mkdir(pathBuffer) != 0)
			{
				MessageBox("Unable to create directory, please make sure the path is correct.", "bScreenDumped->Options", MB_OK|MB_ICONERROR);
				cePath->SetFocus();
				cePath->SetSel(0,cePath->GetWindowTextLength(), TRUE);
				return;
			}
		}
		else
		{
			MessageBox("You chose to not create the directory. Please choose a valid destination before proceeding.", "bScreenDumped->Options", MB_OK|MB_ICONINFORMATION);
			cePath->SetFocus();
			cePath->SetSel(0,cePath->GetWindowTextLength(), TRUE);
			return;
		}
	}

	// Save Settings
	CGlobalSettings gs;
	gs.ReadSettings();
	CButton* genButPtr;

	genButPtr = (CButton*)GetDlgItem(IDC_CHKAUTONAME);
	gs.bAutoName = (genButPtr->GetCheck() == BST_CHECKED) ? TRUE : FALSE;

	genButPtr = (CButton*)GetDlgItem(IDC_CHKHOG);
	gs.bEnableHog = (genButPtr->GetCheck() == BST_CHECKED) ? TRUE : FALSE;

	switch ( GetCheckedRadioButton( IDC_RADIOBMP, IDC_RADIOJPEG ) )
	{
	case IDC_RADIOBMP:
		gs.sEnc = sEncBMP;
		break;

	case IDC_RADIOJPEG:
		gs.sEnc = sEncJPEG;
		char temp[4];
		GetDlgItem(IDC_TXTQUALITY)->GetWindowText(temp, 4);
		gs.lJpgQuality = atol(temp);
		break;

	case IDC_RADIOPNG:
		gs.sEnc = sEncPNG;
		break;

	case IDC_RADIOTIFF:
		gs.sEnc = sEncTIFF;
		break;
	}

	GetDlgItem(IDC_TXTDESTINATION)->GetWindowText(gs.szOutputDir, _MAX_PATH);
	gs.WriteSettings();
	
	// Call default OnOK Event
	OnOK();
}

void OptionsDialog::OnBnClickedCancel()
{
	OnCancel();
}

void OptionsDialog::OnBnClickedRadiobmp()
{
	GetDlgItem(IDC_TXTQUALITY)->EnableWindow(FALSE); 
}

void OptionsDialog::OnBnClickedRadiotiff()
{
	GetDlgItem(IDC_TXTQUALITY)->EnableWindow(FALSE); 
}

void OptionsDialog::OnBnClickedRadiopng()
{
	GetDlgItem(IDC_TXTQUALITY)->EnableWindow(FALSE); 
}

void OptionsDialog::OnBnClickedRadiojpeg()
{
	GetDlgItem(IDC_TXTQUALITY)->EnableWindow(TRUE); 
}

void OptionsDialog::OnBnClickedBtnbrowse()
{
	CFileDialog* cfDlg = new CFileDialog(TRUE, NULL, "DO NOT DELETE THIS TEXT", OFN_HIDEREADONLY|OFN_EXPLORER, "Directories|*.12ewdfvrfter||", this);
	if(cfDlg->DoModal() == IDOK)
	{
		CString csPath = cfDlg->GetPathName();
		csPath = csPath.Left( csPath.ReverseFind('\\') );
		GetDlgItem(IDC_TXTDESTINATION)->SetWindowText(csPath);
	}
	delete cfDlg;
}

BOOL OptionsDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	ShowWindow(SW_NORMAL);
	// Load Settings
	CGlobalSettings gs;
	gs.ReadSettings();
	CButton* radioEncoder;
	// Initialize Controls
	switch(gs.sEnc)
	{
	case sEncBMP:
		radioEncoder = (CButton*)GetDlgItem(IDC_RADIOBMP);
		radioEncoder->SetCheck(BST_CHECKED);
		OnBnClickedRadiobmp();
		break;

	case sEncJPEG:
		radioEncoder = (CButton*)GetDlgItem(IDC_RADIOJPEG);
		radioEncoder->SetCheck(BST_CHECKED);
		OnBnClickedRadiojpeg();
		break;

	case sEncPNG:
		radioEncoder = (CButton*)GetDlgItem(IDC_RADIOPNG);
		radioEncoder->SetCheck(BST_CHECKED);
		OnBnClickedRadiopng();
		break;
		
	case sEncTIFF:
		radioEncoder = (CButton*)GetDlgItem(IDC_RADIOTIFF);
		radioEncoder->SetCheck(BST_CHECKED);
		OnBnClickedRadiotiff();
		break;

	default:
		MessageBox("Encoder Value not found. This is a very unusual error indeed.", "bScreenDumped->OptionsDialog()", MB_OK | MB_ICONWARNING);
		break;
	}
	char buffer[4];
	itoa((int)gs.lJpgQuality, buffer, 10);
	CEdit* ceQuality = (CEdit*) GetDlgItem(IDC_TXTQUALITY);
	ceQuality->SetWindowText(buffer);	
	ceQuality->SetLimitText(3);
	CButton* genBPtr = (CButton*)GetDlgItem(IDC_CHKAUTONAME);
	if(gs.bAutoName)
	{
		genBPtr->SetCheck(BST_CHECKED);
	}
	else
	{
		genBPtr->SetCheck(BST_UNCHECKED);
	}

	genBPtr = (CButton*)GetDlgItem(IDC_CHKHOG);
	if(gs.bEnableHog)
	{
		genBPtr->SetCheck(BST_CHECKED);
	}
	else
	{
		genBPtr->SetCheck(BST_UNCHECKED);
	}

	GetDlgItem(IDC_TXTDESTINATION)->SetWindowText(gs.szOutputDir);

	return TRUE; 
}
