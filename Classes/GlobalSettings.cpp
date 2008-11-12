#include "stdafx.h"
#include <windows.h>
#include <direct.h>
#include <io.h>
#include ".\GlobalSettings.h"

CGlobalSettings::CGlobalSettings()
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

CGlobalSettings::~CGlobalSettings() 
{ }

// Returns a randomly generated filename if bAutoName is true, a user
// selected filename if bAutoname is false. The string will be empty if the user
// cancels out of the save dialog.
// Note: the returning string will contain the full path as well.
// Throws a CFileException exception if something goes wrong.
CString CGlobalSettings::GetNewFileName()
{
    CString fName;
    CString timeStr;
    CString outDir;
    SYSTEMTIME tNow;
    GetLocalTime(&tNow);
    timeStr.Format(_T("img_%u%02u%02u_%02u%02u%02u%03u"), tNow.wYear, tNow.wMonth, tNow.wDay, tNow.wHour, tNow.wMinute, tNow.wSecond, tNow.wMilliseconds);

    // We try and do some sanity validation. I seem to be a big fan of this...
    if(bAutoName)
    {
        outDir = getOutputDir();
        if( CheckCreateDir(outDir) )
        {
            fName = outDir + timeStr;
        }
        else
        {
            AfxThrowFileException(CFileException::badPath, -1, outDir);
        }
    }
    else
    {
        // Get Filename from user. If he cancels out then return empty string.
        CFileDialog cfSaveAs(FALSE, NULL, timeStr, OFN_HIDEREADONLY|OFN_EXPLORER, _T("All Files (*.*)|*.*||"), NULL);
	    if( cfSaveAs.DoModal() == IDOK )
	    {
		    fName = cfSaveAs.GetPathName();
	    }
        else
        {
            fName = _T("");
        }
    }
    return fName;
}

// The only reason you'd have to call this implicitly would be if you for some reason
// want to reload settings from file. 
void CGlobalSettings::ReadSettings()
{
	CString defValue;
	LPTSTR defPtr, outPtr;
    sEnc = (selEncoder)GetPrivateProfileInt(_T("screendump"), _T("Encoder"), 1, m_IniPath);
	lJpgQuality = (long)GetPrivateProfileInt(_T("screendump"), _T("JPGQuality"), 100, m_IniPath);
	bAutoName = (BOOL)GetPrivateProfileInt(_T("screendump"), _T("AutoName"), 1, m_IniPath);
	bEnableHog = (BOOL)GetPrivateProfileInt(_T("screendump"), _T("HogVideo"), 0, m_IniPath);
    bWantClipboard = (BOOL)GetPrivateProfileInt(_T("screendump"), _T("WantClipboard"), 1, m_IniPath);
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

void CGlobalSettings::WriteSettings()
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
}

// Checks if the directory 'path' exists. If it doesn't, it 
// attempts to create it. If creation fails, we return false.
// Otherwise, we return true.
BOOL CGlobalSettings::CheckCreateDir(CString path)
{
    BOOL returnVal = TRUE;
	if(_taccess(path, 0) == -1)
	{
		if(_tmkdir(path) != 0)
		{
			returnVal = FALSE;
		}
	}
    return returnVal;
}

CString CGlobalSettings::getOutputDir()
{
    return szOutputDir;
}

BOOL CGlobalSettings::setOutputDir(CString &sDir)
{
    if( CheckCreateDir(sDir) )
    {
        szOutputDir = sDir;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}