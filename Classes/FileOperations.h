/*
    FileOperations.h
    Several generalized file operations that are done identically in lots of places.
    Others are generalizations that probably didn't belong to they class that contained them
    (example: GenerateAutoFileName() used to be in settings class). 
*/

// Returns a new filename using the timestamp: YYYYMMDD-HHMMSSuuu.ext
CString GenerateAutoFileName();

// TODO Returns filename based on several pre-specified tokens and user constants. (NYI)
CString GenerateTokenizedFileName(CString tokens);

// Checks if the file already exists. Returns TRUE if it does, otherwise FALSE.
BOOL CheckFileExists(CString fileName);

// Checks if the Path exists. If it doesn't it tries to create it. Returns true if the path exists after execution
// (wether it created it or it already existed), and FALSE if by some reason the path didn't exist and creation failed.
BOOL EnsurePathExists(CString path);