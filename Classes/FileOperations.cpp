#include "stdafx.h"
#include <direct.h>
#include <io.h>
#include "FileOperations.h"
#include "GlobalSettings.h"

// Returns a new filename using the timestamp: img_YYYYMMDD_HHMMSSuuu.
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
    while( CheckFileExists(path + timeStr + ext) )
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
    return timeStr + ext;
}

// Opens a File Open dialog for the user. Returns the full path and filename selected by the user, or 
// empty string if the user cancels out.
CString OpenFileDialog()
{
    CString fName;
    CString ext;
    CString Filter;
    GlobalSettings gs;
    
    switch (gs.sEnc)
    {
        case sEncBMP:
            ext = _T("bmp");
            Filter = _T("Bitmap Files (*.bmp)|*.bmp||");
            break;
            
        case sEncJPEG:
            ext = _T("jpg");
            Filter = _T("JPEG Files (*.jpg)|*.jpg||");
            break;
            
        case sEncPNG:
            ext = _T("png");
            Filter = _T("PNG Files (*.png)|*.png||");
            break;
            
        default:
            ext = _T("");
            Filter = _T("All Files (*.*)|*.*||");
            break;
    }
    
    CFileDialog cfSaveAs(FALSE, ext, _T(""), OFN_HIDEREADONLY|OFN_EXPLORER, Filter, NULL, 0, TRUE);
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
