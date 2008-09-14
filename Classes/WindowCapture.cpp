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

BOOL WindowCapture::StartGDI()
{
    lastGdiStatus = GdiplusStartup(&gdiToken, &gdiStartupInput, NULL);
    return (lastGdiStatus == Ok);
}

void WindowCapture::StopGDI()
{
    if(lastGdiStatus == Ok)
    {
        GdiplusShutdown(gdiToken);
        lastGdiStatus = Aborted
    }
}

void WindowCapture::SetEncoder(selEncoder enc, long lQualityVal /* = 100 */)
{
    sEnc = enc;
    lQuality = lQualityVal;
}

selEncoder WindowCapture::GetEncoder()
{
    return sEnc;
}

long WindowCapture::GetQuality()
{
    return lQuality;
}

void WindowCapture::SetQuality(long lQualityVal)
{
    // If an invalid number is specified for quality, default to the highest.
    if( (lQualityVal <= 0) || (lQualityVal > 100))
    {
        lQuality = 100;
    }
    else
    {
        lQuality = lQualityVal;
    }
}

BOOL WindowCapture::CaptureRegion(int xCoord, int yCoord, int iWidth, int iHeight, CString strFilename)
{
    return FALSE; // Not yet implemented.
}

BOOL WindowCapture::CaptureScreen(CString strFilename)
{
	int height = GetSystemMetrics(SM_CYVIRTUALSCREEN);
	int width = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	HDC hhdc = GetDC(NULL); 
	DoCapture(hhdc, 0, 0, width, height, TRUE, strFilename);
    ReleaseDC(NULL, hhdc);
	DeleteDC(hhdc);
    return TRUE;
}

BOOL WindowCapture::CaptureWindow(CString strFilename)
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
	HDC hhdc = GetWindowDC(hWnd);  
	DoCapture(hhdc, 0, 0, rect.Width(), rect.Height(), FALSE, strFilename);
    ReleaseDC(NULL, hhdc);
	DeleteDC(hhdc);
    return TRUE;
}

/**********************************************************************
DoCapture no longer deletes objects it doesn't create. Meaning that
HDC's passed to it MUST be deleted by the caller after this function
returns.
**********************************************************************/
BOOL WindowCapture::DoCapture(HDC &hdc, int xCoord, int yCoord, int iWidth, int iHeight, BOOL bWantOverlayed, CString strFilename)
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

void WindowCapture::DumpImage(Bitmap* aBmp, LPCTSTR filename)
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
	switch (sEnc)
	{
	case sEncBMP:
		fullFilePath += _T(".bmp");
		GetEncoderClsid(_T("image/bmp"), &encoderClsid);
		eParams.Count = 0;
		break;

	case sEncPNG:
		GetEncoderClsid(_T("image/png"), &encoderClsid);
		fullFilePath += _T(".png");
		eParams.Count = 0;		
		break;

	case sEncJPEG:
		GetEncoderClsid(_T("image/jpeg"), &encoderClsid);
		fullFilePath += _T(".jpg");
		eParams.Count = 1;
		eParams.Parameter[0].Guid = EncoderQuality;
		eParams.Parameter[0].NumberOfValues = 1;
		eParams.Parameter[0].Type = EncoderParameterValueTypeLong;
		eParams.Parameter[0].Value = &lQuality;
	    break;
	}
	aBmp->Save(fullFilePath, &encoderClsid, &eParams);
}