#include "stdafx.h"
/* #include <windows.h>
#include ".\Helpers.h"
#include "afxdlgs.h"
#include <io.h>
*/
/*
CString GetNewFilename()
{
    CString strRet;
	SYSTEMTIME tNow;
	GetLocalTime(&tNow);
	strRet.Format(_T("img_%u%02u%02u_%02u%02u%02u%03u"), tNow.wYear, tNow.wMonth, tNow.wDay, tNow.wHour, tNow.wMinute, tNow.wSecond, tNow.wMilliseconds);
    return strRet;
}
*/
/*
BOOL CheckCreateDir(CString path)
{
    BOOL returnVal = TRUE;
    // Check if the directory has been deleted:
	if(_taccess(path, 0) == -1)
	{
		if(_tmkdir(path) != 0)
		{
			returnVal = FALSE;
		}
	}
    return returnVal;
}
*/
/*
CString GetFilenameFromUser()
{
    CString retStr;
    CFileDialog cfSaveAs(FALSE, NULL, GetNewFilename(), OFN_HIDEREADONLY|OFN_EXPLORER, _T("All Files (*.*)|*.*||"), NULL);
	if( cfSaveAs.DoModal() == IDOK )
	{
		retStr = cfSaveAs.GetPathName();
	}
    else
    {
        retStr = _T("");
    }
    return retStr;
}
*/

/*
This was added for the region select dialog, if that ever gets implemented
void SetTransparency(HWND hWnd, long lAlpha)
{
	OSVERSIONINFO osv;
	osv.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osv);
	if(osv.dwMajorVersion < 5)
	{
		long oldStyle = GetWindowLong(hWnd, GWL_EXSTYLE);
        // TODO: Find out why WS_EX_LAYERED shows up as undefined
		SetWindowLong(hWnd, GWL_EXSTYLE, oldStyle | WS_EX_LAYERED);
        // TODO: Find out why LWA_ALPHA shows up as undefined
		SetLayeredWindowAttributes(hWnd, NULL, (BYTE)lAlpha, LWA_ALPHA);
	}
}
*/