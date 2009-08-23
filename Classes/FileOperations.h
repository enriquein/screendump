#pragma once
/*
    FileOperations.h
    Several generalized file operations that are done identically in lots of places.
    Others are generalizations that probably didn't belong to they class that contained them
    (example: GenerateAutoFileName() used to be in settings class). 
*/

// Returns a new filename using the timestamp: img_YYYYMMDD_HHMMSSuuu.
// Provide a path for verifying if a file already exists and increment 1 to the filename.
// This is useful since windows time doesn't update every millisecond.
CString GenerateAutoFileName();

// Opens a File Open dialog for the user. Returns the full path and filename selected by the user, or 
// empty string if the user cancels out.
CString OpenFileDialog();

// TODO Returns filename based on several pre-specified tokens and user constants. (excluding extension) (NYI)
CString GenerateTokenizedFileName(const CString& tokens);

// Checks if the file already exists. Returns TRUE if it does, otherwise FALSE.
BOOL CheckFileExists(const CString& fileName);

// Checks if the Path exists. If it doesn't it tries to create it. Returns true if the path exists after execution
// (wether it created it or it already existed), and FALSE if by some reason the path didn't exist and creation failed.
BOOL EnsurePathExists(const CString& path);

// inline function defs

// TODO Returns filename based on several pre-specified tokens and user constants. (excluding extension) (NYI)
inline CString GenerateTokenizedFileName(const CString& tokens)
{
    return _T("Implement me.");
}

// Checks if the file already exists. Returns TRUE if it does, otherwise FALSE.
inline BOOL CheckFileExists(const CString& fileName)
{
    return _taccess_s(fileName, 0) == 0;
}

// Checks if the Path exists. If it doesn't it tries to create it. Returns true if the path exists after execution
// (wether it created it or it already existed), and FALSE if by some reason the path didn't exist and creation failed.
inline BOOL EnsurePathExists(const CString& path)
{
    BOOL returnVal = TRUE;
	if( !CheckFileExists(path) )
	{
		if(_tmkdir(path) != 0)
		{
			returnVal = FALSE;
		}
	}
    return returnVal;
}