#pragma once


// OptionsDialog dialog
#define OPTIONSDLG_CLOSED (WM_USER + 102)

class OptionsDialog : public CDialog
{
	DECLARE_DYNAMIC(OptionsDialog)

public:
	OptionsDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~OptionsDialog();
	HWND parenthWnd;

// Dialog Data
	enum { IDD = IDD_SETTINGSDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support


	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedRadiobmp();
	afx_msg void OnBnClickedRadiopng();
	afx_msg void OnBnClickedRadiojpeg();
	afx_msg void OnBnClickedBtnbrowse();
	virtual BOOL OnInitDialog();
protected:
	virtual void OnCancel();
	virtual void PostNcDestroy();
};
