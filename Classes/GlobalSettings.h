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
    CString GetNewFileName();
    CString getOutputDir();
    BOOL setOutputDir(CString& sDir);

protected:
    BOOL CheckCreateDir(CString path);
	CString szOutputDir;    
	CString m_IniPath;
};
#endif