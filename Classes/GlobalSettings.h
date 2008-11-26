#pragma once
#ifndef GLOBALSETTINGS_H
#define GLOBALSETTINGS_H
#include "WindowCapture.h"

class GlobalSettings
{
public:
	GlobalSettings();
	~GlobalSettings();

	selEncoder sEnc;
	BOOL bAutoName;
    BOOL bWantClipboard;
	BOOL bEnableHog;
	long lJpgQuality;
	void ReadSettings();
	void WriteSettings();
    CString getOutputDir();
    BOOL setOutputDir(CString& sDir);

protected:
	CString szOutputDir;    
	CString m_IniPath;
};
#endif