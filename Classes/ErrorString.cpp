#include "stdafx.h"
#include "ErrorString.h"

/****************************************************************************
*                                 ErrorString
* Inputs:
*       DWORD err: Error code
* Result: CString
*       String message
****************************************************************************/
CString ErrorString(DWORD err)
{
    CString Error;
    LPTSTR s;
    if(::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, err, 0, (LPTSTR)&s, 0, NULL) == 0)
    { /* failed */
        // Unknown error code %08x (%d)
        CString errStr;
        errStr.Format(_T("Unknown error: %d"), LOWORD(err));
        Error = errStr;
    }  
    else
    { /* success */
        LPTSTR p = _tcschr(s, _T('\r'));
        if(p != NULL)
        { /* lose CRLF */
            *p = _T('\0');
        }  
        Error = s;
        ::LocalFree(s);
    }  
    return Error;
} 
