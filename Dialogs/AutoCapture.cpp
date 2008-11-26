#include "stdafx.h"
#include "..\Classes\screendump.h"
#include ".\AutoCapture.h"
#include "..\Classes\WindowCapture.h"

UINT AutoCapture::UWM_TIMER_AC = ::RegisterWindowMessage(_T("UWM_TIMER_AC-{6B26ED52-0908-422b-9944-17DCC2EB7A40}"));
UINT AutoCapture::UWM_CAPTUREWINDOW = ::RegisterWindowMessage(_T("UWM_CAPTURESCREEN-{8BCA6B45-C3E5-4c08-8D1D-C6CF1CE4E6F0}"));
UINT AutoCapture::UWM_CAPTURESCREEN = ::RegisterWindowMessage(_T("UWM_CAPTUREWINDOW-{15C4F437-8121-4530-BC07-FDB0E695012A}"));


IMPLEMENT_DYNAMIC(AutoCapture, CDialog)
AutoCapture::AutoCapture(CWnd* pParent /*=NULL*/)
	: CDialog(AutoCapture::IDD, pParent)
{
}

AutoCapture::~AutoCapture()
{
}

void AutoCapture::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, ID_TXTACDELAY, c_txtACDelay);
    DDX_Control(pDX, ID_OPTSECONDS, c_optSeconds);
    DDX_Control(pDX, ID_OPTMILLISECONDS, c_optMilliseconds);
    DDX_Control(pDX, ID_OPTFULLSCREEN, c_optFullScreen);
    DDX_Control(pDX, ID_OPTFOREGROUND, c_optForeground);
    DDX_Control(pDX, ID_BTNSTART, c_btnStart);
}


BEGIN_MESSAGE_MAP(AutoCapture, CDialog)
	ON_MESSAGE(WM_TIMER, OnTimer)
	ON_BN_CLICKED(ID_BTNSTART, OnBnClickedOk)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// AutoCapture message handlers

void AutoCapture::OnBnClickedOk() // Start Capture was clicked
{
	// Check if we're actually stopping the capture:
	CString tmpText;
    c_btnStart.GetWindowText(tmpText);
	if(tmpText != _T("Start AutoCapture"))
	{
		// Stopping Capture
        KillTimer(UWM_TIMER_AC);
		EnableControls(TRUE);
		c_btnStart.SetWindowText(_T("Start AutoCapture"));
		return;
	}

	// Validation
	UINT iDelay;
    c_txtACDelay.GetWindowText(tmpText);
    tmpText = tmpText.Trim();
	if(tmpText.GetLength() == 0)
	{
		MessageBox(_T("Please enter the amount of time to wait until screenshots."), _T("screendump->AutoCapture"), MB_OK|MB_ICONERROR);
		c_txtACDelay.SetFocus();
		return;
	}
    c_txtACDelay.GetWindowText(tmpText);
	iDelay = (UINT)_ttoi(tmpText);
    if(c_optSeconds.GetCheck() == BST_CHECKED) 
	{
		// User chose seconds
		if(iDelay > 60)
		{
			MessageBox(_T("Please enter a number between 1 and 60."), _T("screendump->AutoCapture"), MB_OK|MB_ICONERROR);
			c_txtACDelay.SetFocus();
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
			MessageBox(_T("Please enter a number between 250 and 60000."), _T("screendump->AutoCapture"), MB_OK|MB_ICONERROR);
			c_txtACDelay.SetFocus();
			return;
		}
	}
	// End Validation

    if(c_optFullScreen.GetCheck() == BST_CHECKED) 
	{
		// User chose fullscreen mode
		bCatchForeground = false;
	}
	else
	{
		// User chose active window mode
		bCatchForeground = true;
	}
	EnableControls(false);
	c_btnStart.SetWindowText(_T("Stop AutoCapture"));
	::SetTimer(this->m_hWnd, UWM_TIMER_AC, iDelay, NULL);
	ShowWindow(SW_SHOWMINIMIZED);
}

BOOL AutoCapture::OnInitDialog()
{
	CDialog::OnInitDialog();
    c_optFullScreen.SetCheck(BST_CHECKED);
    c_optSeconds.SetCheck(BST_CHECKED);
    c_txtACDelay.SetWindowText(_T("5"));
	return TRUE;  // return TRUE unless you set the focus to a control
}

inline LRESULT AutoCapture::OnTimer(WPARAM wParam, LPARAM lParam)
{
	::SendMessage(  GetParent()->m_hWnd, 
                    bCatchForeground ? UWM_CAPTUREWINDOW : UWM_CAPTURESCREEN,
                    (WPARAM) TRUE, 
				    0);
    return 0;
}
