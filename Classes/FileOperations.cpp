#include "stdafx.h"
#include "FileOperations.h"

// Returns a new filename using the timestamp: YYYYMMDD-HHMMSSuuu.ext
CString GenerateAutoFileName()
{
    return _T("Implement me.");
}

// TODO Returns filename based on several pre-specified tokens and user constants. (NYI)
CString GenerateTokenizedFileName(CString tokens)
{
    return _T("Implement me.");
}

// Checks if the file already exists. Returns TRUE if it does, otherwise FALSE.
BOOL CheckFileExists(CString fileName)
{
    return FALSE;
}

// Checks if the Path exists. If it doesn't it tries to create it. Returns true if the path exists after execution
// (wether it created it or it already existed), and FALSE if by some reason the path didn't exist and creation failed.
BOOL EnsurePathExists(CString path)
{
    return FALSE;
}