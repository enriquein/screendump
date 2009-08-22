#include "stdafx.h"
#include <windows.h>
#include ".\GlobalSettings.h"
#include ".\FileOperations.h"

// TODO Add a way to run from USB or other portable device.

GlobalSettings::GlobalSettings()
{
	// Get INI File path
	OSVERSIONINFO osv;
	osv.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osv);
	if( osv.dwMajorVersion >= 5)
	{
        LPTSTR p = m_IniPath.GetBuffer(_MAX_PATH);
		SHGetSpecialFolderPath(NULL, p, CSIDL_APPDATA, FALSE);
        m_IniPath.ReleaseBuffer();
        m_IniPath += _T("\\screendump\\");
		if(_taccess(m_IniPath, 0) == -1)
		{
			_tmkdir(m_IniPath);
		}
        m_IniPath += _T("screendump.ini");
	}
	else
	{
        LPTSTR p = m_IniPath.GetBuffer(_MAX_PATH); 
		_tgetcwd(p, _MAX_PATH);
        m_IniPath.ReleaseBuffer();
        m_IniPath += _T("\\screendump.ini");
	}
	ReadSettings();
}

GlobalSettings::~GlobalSettings() 
{ }

// The only reason you'd have to call this implicitly would be if you for some reason
// want to reload settings from file. 
void GlobalSettings::ReadSettings()
{
	CString defValue;
	LPTSTR defPtr, outPtr;
    sEnc = (selEncoder)GetPrivateProfileInt(_T("screendump"), _T("Encoder"), 1, m_IniPath);
	lJpgQuality = (long)GetPrivateProfileInt(_T("screendump"), _T("JPGQuality"), 100, m_IniPath);
	bAutoName = (BOOL)GetPrivateProfileInt(_T("screendump"), _T("AutoName"), 1, m_IniPath);
	bEnableHog = (BOOL)GetPrivateProfileInt(_T("screendump"), _T("HogVideo"), 0, m_IniPath);
    bWantClipboard = (BOOL)GetPrivateProfileInt(_T("screendump"), _T("WantClipboard"), 1, m_IniPath);
    bWantCursor = (BOOL)GetPrivateProfileInt(_T("screendump"), _T("WantCursor"), 1, m_IniPath);
    defPtr = defValue.GetBuffer(_MAX_PATH);
	_tgetcwd(defPtr, _MAX_PATH);
    defValue.ReleaseBuffer();
    outPtr = szOutputDir.GetBuffer(_MAX_PATH);
	GetPrivateProfileString(_T("screendump"), _T("OutDir"), defValue, outPtr, _MAX_PATH, m_IniPath);
    szOutputDir.ReleaseBuffer();
    // Ok so lets check if the directory has an ending \ to avoid some unexpected stuff
    if( szOutputDir.Right(1) != _T("\\") )
    {
        szOutputDir += _T("\\");
    }
}

void GlobalSettings::WriteSettings()
{
    CString buffer;
    buffer.Format(_T("%d"), sEnc);
	WritePrivateProfileString(_T("screendump"), _T("Encoder"), buffer, m_IniPath);
    buffer.Format(_T("%d"), lJpgQuality);
	WritePrivateProfileString(_T("screendump"), _T("JPGQuality"), buffer, m_IniPath);
    buffer.Format(_T("%d"), bAutoName);
	WritePrivateProfileString(_T("screendump"), _T("AutoName"), buffer, m_IniPath);
	WritePrivateProfileString(_T("screendump"), _T("OutDir"), szOutputDir, m_IniPath);
    buffer.Format(_T("%d"), bEnableHog);
	WritePrivateProfileString(_T("screendump"), _T("HogVideo"), buffer, m_IniPath);
    buffer.Format(_T("%d"), bWantClipboard);
	WritePrivateProfileString(_T("screendump"), _T("WantClipboard"), buffer, m_IniPath);
	buffer.Format(_T("%d"), bWantCursor);
	WritePrivateProfileString(_T("screendump"), _T("WantCursor"), buffer, m_IniPath);
}

