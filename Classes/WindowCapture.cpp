#include "stdafx.h"
#include "afxwin.h"
#include "WindowCapture.h"
#include "gdiplus.h"
using namespace Gdiplus;

#ifndef CAPTUREBLT
#define CAPTUREBLT  (DWORD)0x40000000
#endif

#ifndef SM_CYVIRTUALSCREEN
#define SM_CYVIRTUALSCREEN 79
#endif

#ifndef SM_CXVIRTUALSCREEN
#define SM_CXVIRTUALSCREEN 78
#endif

// Default constructor, throws CResourceException if GDIStartup failed.
WindowCapture::WindowCapture() 
{
    lastGdiStatus = Aborted; // Initialized to anything not "Ok" so that StartGDI is the *only* one to be able to change it to "Ok".
    sEnc = sEncJPEG;
    lQuality = 100;
    bUseClipboard = TRUE;
    gdiStartupInput.GdiplusVersion = 1; 
    gdiStartupInput.DebugEventCallback = NULL;
    gdiStartupInput.SuppressBackgroundThread = FALSE;
    gdiStartupInput.SuppressExternalCodecs = FALSE;
    if(!StartGDI())
    {   
        // throw exception so our caller knows gdi startup failed.
        AfxThrowResourceException();
    }
}

WindowCapture::~WindowCapture()
{
    StopGDI();
}

BOOL WindowCapture::CaptureScreen(const CString& strFilename)
{
	int height = GetSystemMetrics(SM_CYVIRTUALSCREEN);
	int width = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	HDC hhdc = GetDC(NULL); 
	DoCapture(hhdc, 0, 0, width, height, TRUE, strFilename);
    ReleaseDC(NULL, hhdc);
	DeleteDC(hhdc);
    return TRUE;
}

BOOL WindowCapture::CaptureWindow(const CString& strFilename)
{
    HWND hWnd = NULL;
    hWnd = ::GetForegroundWindow();   
    if(!hWnd)
	{
        return FALSE;
	}
    CRect rect;
    GetWindowRect(hWnd, &rect);
	rect.NormalizeRect();
	HDC hhdc = GetDC(NULL);  
	DoCapture(hhdc, rect.left, rect.top, rect.Width(), rect.Height(), TRUE, strFilename);
    ReleaseDC(NULL, hhdc);
	DeleteDC(hhdc);
    return TRUE;
}

/**********************************************************************
DoCapture no longer deletes objects it doesn't create. Meaning that
HDC's passed to it MUST be deleted by the caller after this function
returns.
**********************************************************************/
BOOL WindowCapture::DoCapture(const HDC &hdc, const int& xCoord, const int& yCoord, const int& iWidth, const int& iHeight, const BOOL& bWantOverlayed, const CString& strFilename)
{
    long bltFlags;
	bltFlags = bWantOverlayed ? (CAPTUREBLT|SRCCOPY) : SRCCOPY; // CAPTUREBLT means that we want to include windows that are on top of ours.
	HDC hDest = CreateCompatibleDC(hdc);
	HBITMAP hbDesktop = CreateCompatibleBitmap( hdc, iWidth, iHeight ); 
	SelectObject(hDest, hbDesktop);
	BitBlt(hDest, 0,0, iWidth, iHeight, hdc, xCoord, yCoord, bltFlags);
	HPALETTE hPalette = (HPALETTE)GetCurrentObject(hdc, OBJ_PAL);
	Bitmap myBitMap(hbDesktop, hPalette);
	DumpImage(&myBitMap, strFilename);
    if(bUseClipboard)
    {
        if(OpenClipboard(NULL))
        {
            EmptyClipboard();
            SetClipboardData(CF_BITMAP, hbDesktop);
            CloseClipboard();
        }
    }
    DeleteObject(hbDesktop);
	DeleteObject(hPalette);
    DeleteDC(hDest);
    return TRUE;
}

int WindowCapture::GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
   UINT  num = 0;          // number of image encoders
   UINT  size = 0;         // size of the image encoder array in bytes

   ImageCodecInfo* pImageCodecInfo = NULL;

   GetImageEncodersSize(&num, &size);
   if(size == 0)
      return -1;  // Failure

   pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
   if(pImageCodecInfo == NULL)
      return -1;  // Failure

   GetImageEncoders(num, size, pImageCodecInfo);

   for(UINT j = 0; j < num; ++j)
   {
      if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
      {
         *pClsid = pImageCodecInfo[j].Clsid;
         free(pImageCodecInfo);
         return j;  // Success
      }    
   }

   free(pImageCodecInfo);
   return -1;  // Failure
}

void WindowCapture::DumpImage(Bitmap* aBmp, const CString& filename)
{
	// MSDN NOTE:
	//When you create an EncoderParameters object, you must allocate enough memory
	//to hold all of the EncoderParameter objects that will eventually be placed in
	//the array. For example, if you want to create an EncoderParameters object that will
	//hold an array of five EncoderParameter objects, you should use code similar to the following:

	//	EncoderParameters* pEncoderParameters = (EncoderParameters*)
	//	malloc(sizeof(EncoderParameters) + 4 * sizeof(EncoderParameter));
    
    CString fullFilePath(filename);
	EncoderParameters eParams; // Since we use only 1 parameter we don't need the fancy malloc stuff
	CLSID encoderClsid;
	BOOL extAppend = FALSE;
	
	// Check if the file extension was already provided to avoid double appending it.
	switch (sEnc)
	{
	case sEncBMP:
	    if (fullFilePath.GetLength() > 4)
	    {
	        extAppend = (fullFilePath.Right(4) == _T(".bmp")) ? FALSE : TRUE;
	    }
	    else
	    {
	        extAppend = TRUE;
	    }

		fullFilePath += extAppend ? _T(".bmp") : _T("");		
		GetEncoderClsid(_T("image/bmp"), &encoderClsid);
		eParams.Count = 0;
		break;

	case sEncPNG:
	    if (fullFilePath.GetLength() > 4)
	    {
	        extAppend = (fullFilePath.Right(4) == _T(".png")) ? FALSE : TRUE;
	    }
	    else
	    {
	        extAppend = TRUE;
	    }

		fullFilePath += extAppend ? _T(".png") : _T("");	
		GetEncoderClsid(_T("image/png"), &encoderClsid);
		eParams.Count = 0;		
		break;

	case sEncJPEG:
        if (fullFilePath.GetLength() > 4)
	    {
	        extAppend = (fullFilePath.Right(4) == _T(".jpg")) ? FALSE : TRUE;
	    }
	    else
	    {
	        extAppend = TRUE;
	    }

		fullFilePath += extAppend ? _T(".jpg") : _T("");	
		GetEncoderClsid(_T("image/jpeg"), &encoderClsid);
		eParams.Count = 1;
		eParams.Parameter[0].Guid = EncoderQuality;
		eParams.Parameter[0].NumberOfValues = 1;
		eParams.Parameter[0].Type = EncoderParameterValueTypeLong;
		eParams.Parameter[0].Value = &lQuality;
	    break;
	}

    // TODO add exception handling in case of ReadOnly path or Full Disk etc.
    // hint: declare a variable of type Status and then do a switch based on that. 
    aBmp->Save(fullFilePath, &encoderClsid, &eParams);
}