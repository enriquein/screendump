#include "StdAfx.h"
#include ".\globalsettings.h"
#include <direct.h>
#include <io.h>

CGlobalSettings::CGlobalSettings(void)
{
	// Get INI File path
	OSVERSIONINFO osv;
	GetVersionEx(&osv);
	if( osv.dwMajorVersion > 4)
	{
		SHGetSpecialFolderPath(NULL, m_IniPath, CSIDL_APPDATA, FALSE);
		sprintf(m_IniPath, "%s\\bScreenDumped\\", m_IniPath);
		if(_access(m_IniPath, 0) == -1)
		{
			_mkdir(m_IniPath);
		}
		sprintf(m_IniPath, "%sbS.ini", m_IniPath);
	}
	else
	{
		_getcwd(m_IniPath, _MAX_PATH);
        sprintf(m_IniPath, "%s\\bS.ini", m_IniPath);
	}
}

CGlobalSettings::~CGlobalSettings(void)
{
}

void CGlobalSettings::ReadSettings()
{
	char defValue[_MAX_PATH];
	_getcwd(defValue, _MAX_PATH);
	sEnc = (selEncoder)GetPrivateProfileInt("bScreenDumped", "Encoder", 1, m_IniPath);
	lJpgQuality = (long)GetPrivateProfileInt("bScreenDumped", "JPGQuality", 100, m_IniPath);
	bAutoName = (BOOL)GetPrivateProfileInt("bScreenDumped", "AutoName", 1, m_IniPath);
	GetPrivateProfileString("bScreenDumped", "OutDir", defValue, szOutputDir, _MAX_PATH, m_IniPath);
}

void CGlobalSettings::WriteSettings()
{
	char buffer[_MAX_PATH];
	itoa((int)sEnc, buffer, 10);
	WritePrivateProfileString("bScreenDumped", "Encoder", buffer, m_IniPath);
	itoa((int)lJpgQuality, buffer, 10);
	WritePrivateProfileString("bScreenDumped", "JPGQuality", buffer, m_IniPath);
	itoa((int)bAutoName, buffer, 10);
	WritePrivateProfileString("bScreenDumped", "AutoName", buffer, m_IniPath);
	WritePrivateProfileString("bScreenDumped", "OutDir", szOutputDir, m_IniPath);
}