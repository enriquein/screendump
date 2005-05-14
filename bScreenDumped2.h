// bScreenDumped2.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#ifndef BSCREENDUMPED2
#define BSCREENDUMPED2
#include "resource.h"		// main symbols

// CbScreenDumped2App:
// See bScreenDumped2.cpp for the implementation of this class
//

class CbScreenDumped2App : public CWinApp
{
public:
	CbScreenDumped2App();


// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CbScreenDumped2App theApp;

#endif