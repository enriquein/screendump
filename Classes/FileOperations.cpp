#include "stdafx.h"
#include <direct.h>
#include <io.h>
#include "FileOperations.h"
#include "GlobalSettings.h"

// Returns a new filename using the timestamp: img_YYYYMMDD_HHMMSSuuu (does not include extension).
// Verifies if a file already exists with the generated filename, if it does we increment the filename.
CString GenerateAutoFileName()
{
    GlobalSettings gs;
    CString timeStr;
    CString path;
    SYSTEMTIME tNow;
    CString ext;  
      
    path = gs.bAutoName ? gs.getOutputDir() : _T("\\");
    GetLocalTime(&tNow);
    timeStr.Format(_T("img_%u%02u%02u_%02u%02u%02u%03u"), tNow.wYear, tNow.wMonth, tNow.wDay, tNow.wHour, tNow.wMinute, tNow.wSecond, tNow.wMilliseconds);

    // We check which encoder is selected to know what extension to use
    switch (gs.sEnc)
    {
        case sEncBMP:
            ext = _T(".bmp");
            break;
            
        case sEncJPEG:
            ext = _T(".jpg");
            break;
            
        case sEncPNG:
            ext = _T(".png");
            break;
            
        default:
            ext = _T("");
            break;
    }
    
    // Check if the file already exists
    while( !CheckFileExists(path + timeStr + ext) )
    {
        if (tNow.wMilliseconds == 999)
        {
            break;
        }
        else
        {    
            tNow.wMilliseconds += 1;
            timeStr.Format(_T("img_%u%02u%02u_%02u%02u%02u%03u"), tNow.wYear, tNow.wMonth, tNow.wDay, tNow.wHour, tNow.wMinute, tNow.wSecond, tNow.wMilliseconds);
        }
    }
    return timeStr;
}

// Opens a File Open dialog for the user. Returns the full path and filename selected by the user, or 
// empty string if the user cancels out.
CString OpenFileDialog()
{
    CString fName;
    CFileDialog cfSaveAs(FALSE, NULL, GenerateAutoFileName(), OFN_HIDEREADONLY|OFN_EXPLORER, _T("All Files (*.*)|*.*||"), NULL);
    if( cfSaveAs.DoModal() == IDOK )
    {
	    fName = cfSaveAs.GetPathName();
    }
    else
    {
        fName = _T("");
    }
    return fName;
}

// TODO Returns filename based on several pre-specified tokens and user constants. (excluding extension) (NYI)
CString GenerateTokenizedFileName(CString tokens)
{
    return _T("Implement me.");
}

// Checks if the file already exists. Returns TRUE if it does, otherwise FALSE.
BOOL CheckFileExists(CString fileName)
{
    return _taccess(fileName, 0) == 0;
}

// Checks if the Path exists. If it doesn't it tries to create it. Returns true if the path exists after execution
// (wether it created it or it already existed), and FALSE if by some reason the path didn't exist and creation failed.
BOOL EnsurePathExists(CString path)
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