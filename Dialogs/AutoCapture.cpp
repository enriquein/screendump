#include "stdafx.h"
#include "..\Classes\bScreenDumped2.h"
#include ".\AutoCapture.h"
#include "..\Helpers\WindowCapture.h"

// CAutoCapture dialog

IMPLEMENT_DYNAMIC(CAutoCapture, CDialog)
CAutoCapture::CAutoCapture(CWnd* pParent /*=NULL*/)
	: CDialog(CAutoCapture::IDD, pParent)
{
}

CAutoCapture::~CAutoCapture()
{
}

void CAutoCapture::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAutoCapture, CDialog)
	ON_MESSAGE(WM_TIMER, OnTimer)
	ON_BN_CLICKED(ID_BTNSTART, OnBnClickedOk)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CAutoCapture message handlers

void CAutoCapture::OnBnClickedOk() // Start Capture was clicked
{
	// Check if we're actually stopping the capture:
	CString butText;
	GetDlgItem(ID_BTNSTART)->GetWindowText(butText);
	if(butText != "Start AutoCapture")
	{
		// Stopping Capture
		KillTimer(TIMERAUTOCAPTURE);
		EnableControls(TRUE);
		GetDlgItem(ID_BTNSTART)->SetWindowText("Start AutoCapture");
		return;
	}

	// Validation
	UINT iDelay;
	if(GetDlgItem(ID_TXTACDELAY)->GetWindowTextLength() == 0)
	{
		MessageBox("Please enter the amount of time to wait until screenshots.", "bScreenDumped->AutoCapture", MB_OK|MB_ICONERROR);
		GetDlgItem(ID_TXTACDELAY)->SetFocus();
		return;
	}
	CString secVal;
	GetDlgItem(ID_TXTACDELAY)->GetWindowText(secVal);
	iDelay = (UINT)atoi(secVal);
	CButton* radioButton;
	radioButton = (CButton*)GetDlgItem(ID_OPTSECONDS);
	if(radioButton->GetCheck() == BST_CHECKED) 
	{
		// User chose seconds
		if(iDelay > 60)
		{
			MessageBox("Please enter a number between 1 and 60.", "bScreenDumped->AutoCapture", MB_OK|MB_ICONERROR);
			GetDlgItem(ID_TXTACDELAY)->SetFocus();
			return;
		}
		else
		{
			iDelay *= 1000;
		}
	}
	else
	{
		// User chose milliseconds
		if( (iDelay > 60000) || (iDelay < 250) )
		{
			MessageBox("Please enter a number between 250 and 60000.", "bScreenDumped->AutoCapture", MB_OK|MB_ICONERROR);
			GetDlgItem(ID_TXTACDELAY)->SetFocus();
			return;
		}
	}
	radioButton = (CButton*)GetDlgItem(ID_OPTFULLSCREEN);
	// End Validation

	if(radioButton->GetCheck() == BST_CHECKED) 
	{
		// User chose fullscreen mode
		bCatchForeground = FALSE;
	}
	else
	{
		// User chose active window mode
		bCatchForeground = TRUE;
	}
	EnableControls(FALSE);
	GetDlgItem(ID_BTNSTART)->SetWindowText("Stop AutoCapture");
	::SetTimer(this->m_hWnd, TIMERAUTOCAPTURE, iDelay, NULL);
	ShowWindow(SW_SHOWMINIMIZED);
}

LRESULT CAutoCapture::OnTimer(WPARAM wParam, LPARAM lParam)
{
	if(bCatchForeground)
	{
		CaptureWindow();
	}
	else
	{
		CaptureScreen();
	}
	return 0;
}

void CAutoCapture::EnableControls(BOOL bEnable)
{
	if(bEnable)
	{	
		GetDlgItem(ID_TXTACDELAY)->EnableWindow(TRUE);
		GetDlgItem(ID_OPTFOREGROUND)->EnableWindow(TRUE);
		GetDlgItem(ID_OPTFULLSCREEN)->EnableWindow(TRUE);
		GetDlgItem(ID_OPTSECONDS)->EnableWindow(TRUE);
		GetDlgItem(ID_OPTMILLISECONDS)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(ID_TXTACDELAY)->EnableWindow(FALSE);
		GetDlgItem(ID_OPTFOREGROUND)->EnableWindow(FALSE);
		GetDlgItem(ID_OPTFULLSCREEN)->EnableWindow(FALSE);
		GetDlgItem(ID_OPTSECONDS)->EnableWindow(FALSE);
		GetDlgItem(ID_OPTMILLISECONDS)->EnableWindow(FALSE);
	}
}

BOOL CAutoCapture::OnInitDialog()
{
	CDialog::OnInitDialog();
	CButton* cbut;
	cbut = (CButton*)GetDlgItem(ID_OPTFULLSCREEN);
	cbut->SetCheck(BST_CHECKED);
	cbut = (CButton*)GetDlgItem(ID_OPTSECONDS);
	cbut->SetCheck(BST_CHECKED);
	cbut = NULL;
	GetDlgItem(ID_TXTACDELAY)->SetWindowText("5");
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CAutoCapture::OnCancel()
{
	KillTimer(TIMERAUTOCAPTURE);
	CDialog::OnCancel();
}

void CAutoCapture::OnClose()
{
	OnCancel();
}
