#include "stdafx.h"
#include <windows.h>
#include <direct.h>
#include <io.h>
#include ".\GlobalSettings.h"

CGlobalSettings::CGlobalSettings(void)
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
        m_IniPath += _T("\\bScreenDumped\\");
		if(_taccess(m_IniPath, 0) == -1)
		{
			_tmkdir(m_IniPath);
		}
        m_IniPath += _T("%sbS.ini");
	}
	else
	{
        LPTSTR p = m_IniPath.GetBuffer(_MAX_PATH); 
		_tgetcwd(p, _MAX_PATH);
        m_IniPath.ReleaseBuffer();
        m_IniPath += _T("\\bS.ini");
	}
}

CGlobalSettings::~CGlobalSettings(void)
{
}

void CGlobalSettings::ReadSettings()
{
	CString defValue;
	LPTSTR defPtr, outPtr;
    sEnc = (selEncoder)GetPrivateProfileInt(_T("bScreenDumped2"), _T("Encoder"), 1, m_IniPath);
	lJpgQuality = (long)GetPrivateProfileInt(_T("bScreenDumped2"), _T("JPGQuality"), 100, m_IniPath);
	bAutoName = (bool)GetPrivateProfileInt(_T("bScreenDumped2"), _T("AutoName"), 1, m_IniPath);
	bEnableHog = (bool)GetPrivateProfileInt(_T("bScreenDumped2"), _T("HogVideo"), 0, m_IniPath);
    defPtr = defValue.GetBuffer(_MAX_PATH);
	_tgetcwd(defPtr, _MAX_PATH);
    defValue.ReleaseBuffer();
    outPtr = szOutputDir.GetBuffer(_MAX_PATH);
	GetPrivateProfileString(_T("bScreenDumped2"), _T("OutDir"), defValue, outPtr, _MAX_PATH, m_IniPath);
    szOutputDir.ReleaseBuffer();
}

void CGlobalSettings::WriteSettings()
{
    CString buffer;
    buffer.Format(_T("%d"), sEnc);
	WritePrivateProfileString(_T("bScreenDumped2"), _T("Encoder"), buffer, m_IniPath);
    buffer.Format(_T("%d"), lJpgQuality);
	WritePrivateProfileString(_T("bScreenDumped2"), _T("JPGQuality"), buffer, m_IniPath);
    buffer.Format(_T("%d"), bAutoName);
	WritePrivateProfileString(_T("bScreenDumped2"), _T("AutoName"), buffer, m_IniPath);
	WritePrivateProfileString(_T("bScreenDumped2"), _T("OutDir"), szOutputDir, m_IniPath);
    buffer.Format(_T("%d"), bEnableHog);
	WritePrivateProfileString(_T("bScreenDumped2"), _T("HogVideo"), buffer, m_IniPath);
}