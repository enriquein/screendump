#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#ifndef screendump
#define screendump
#include "..\res\resource.h"		

class screendumpApp : public CWinApp
{
public:
	screendumpApp();
	public:
	virtual BOOL InitInstance();
	DECLARE_MESSAGE_MAP()
};

extern screendumpApp theApp;

#endif