#include "stdafx.h"
#include "bScreenDumped2.h"
#include "bScreenDumped2Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CbScreenDumped2Dlg::CbScreenDumped2Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CbScreenDumped2Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CbScreenDumped2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CbScreenDumped2Dlg, CDialog)
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// CbScreenDumped2Dlg message handlers

BOOL CbScreenDumped2Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	return TRUE;  // return TRUE  unless you set the focus to a control
}
