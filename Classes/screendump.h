#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#ifndef screendump
#define screendump
#include "..\res\resource.h"		

class CscreendumpApp : public CWinApp
{
public:
	CscreendumpApp();
	public:
	virtual BOOL InitInstance();
	DECLARE_MESSAGE_MAP()
};

extern CscreendumpApp theApp;

#endif