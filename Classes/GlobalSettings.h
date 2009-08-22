#pragma once
#ifndef GLOBALSETTINGS_H
#define GLOBALSETTINGS_H
#include "EncoderParameterTypes.h"
#include "FileOperations.h"

class GlobalSettings
{
public:
	GlobalSettings();
	~GlobalSettings();

	selEncoder sEnc;
	BOOL bAutoName;
    BOOL bWantClipboard;
	BOOL bEnableHog;
	BOOL bWantCursor;
	long lJpgQuality;
	void ReadSettings();
	void WriteSettings();
    CString getOutputDir();
    BOOL setOutputDir(const CString& sDir);

protected:
	CString szOutputDir;    
	CString m_IniPath;
};

// inline defs
inline CString GlobalSettings::getOutputDir()
{
    return szOutputDir;
}

inline BOOL GlobalSettings::setOutputDir(const CString &sDir)
{
    if( EnsurePathExists(sDir) )
    {
        szOutputDir = sDir;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
#endif