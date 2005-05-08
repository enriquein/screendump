#include "stdafx.h"
#include "gdiplus.h"
#include "WindowCapture.h"
using namespace Gdiplus;

/*
HBITMAP CaptureDesktop()
{
    HWND hWnd = NULL;

    hWnd = GetDesktopWindow();          // Get handle to desktop window.

    return CaptureWindow(hWnd, FALSE);  // Capture an image of this window.
}

HBITMAP CaptureForegroundWindow(BOOL bClientAreaOnly)
{
    HWND hWnd = NULL;

    hWnd = ::GetForegroundWindow();             // Get the foreground window.

    return CaptureWindow(hWnd, bClientAreaOnly);// Capture an image of this window.
}

HBITMAP CaptureWindow(HWND hWnd, BOOL bClientAreaOnly)
{
    if(!hWnd)
        return NULL;

    HDC hdc;
    RECT rect;
    if(bClientAreaOnly)
    {
        hdc = GetDC(hWnd);
        GetClientRect(hWnd, &rect);
    }
    else
    {
        hdc = GetWindowDC(hWnd);
        GetWindowRect(hWnd, &rect);
    }

    if(!hdc)
        return NULL;

    HDC hMemDC = CreateCompatibleDC(hdc);
    if(hMemDC == NULL)
        return NULL;

    SIZE size;
    size.cx = rect.right - rect.left;
    if(rect.right < rect.left)
        size.cx = -size.cx;
    size.cy = rect.bottom - rect.top;
    if(rect.bottom < rect.top)
        size.cy = -size.cy;

    HBITMAP hDDBmp = CreateCompatibleBitmap(hdc, size.cx, size.cy);
    if(hDDBmp == NULL)
    {
        DeleteDC(hMemDC);
        ReleaseDC(hWnd, hdc);
        return NULL;
    }

    HBITMAP hOldBmp = static_cast<HBITMAP>(SelectObject(hMemDC, hDDBmp));
    BitBlt(hMemDC, 0, 0, size.cx, size.cy, hdc, 0, 0, SRCCOPY);
    SelectObject(hMemDC, hOldBmp);


    HBITMAP hBmp = static_cast<HBITMAP>(CopyImage(hDDBmp,
                                                    IMAGE_BITMAP,
                                                    0,
                                                    0,
                                                    LR_CREATEDIBSECTION));



	HPALETTE hPalette = (HPALETTE)GetCurrentObject(hdc, OBJ_PAL);
	Bitmap *myBitMap = new Bitmap(hBmp, hPalette);
	CLSID encoderClsid;
	GetEncoderClsid(L"image/png", &encoderClsid);
	myBitMap->Save(L"desktop.png", &encoderClsid);

    DeleteObject(hDDBmp);
    DeleteDC(hMemDC);
    ReleaseDC(hWnd, hdc);
	DeleteObject(hBmp);
	myBitMap = NULL;
    return hBmp;
}
*/

BOOL CaptureWindow()
{
    HWND hWnd = NULL;
    hWnd = ::GetForegroundWindow();         
    if(!hWnd)
	{
        return FALSE;
	}
    HDC hdc;
    RECT rect;
    hdc = GetWindowDC(hWnd);
    GetWindowRect(hWnd, &rect);
    if(!hdc)
	{
        return FALSE;
	}
    HDC hMemDC = CreateCompatibleDC(hdc);
    if(hMemDC == NULL)
	{    
		return FALSE;
	}
	SIZE size;
    size.cx = rect.right - rect.left;
    if(rect.right < rect.left)
	{
        size.cx = -size.cx;
	}
	size.cy = rect.bottom - rect.top;
    if(rect.bottom < rect.top)
	{
        size.cy = -size.cy;
	}
    HBITMAP hDDBmp = CreateCompatibleBitmap(hdc, size.cx, size.cy);
    if(hDDBmp == NULL)
    {
        DeleteDC(hMemDC);
        ReleaseDC(hWnd, hdc);
        return NULL;
    }
    HBITMAP hOldBmp = static_cast<HBITMAP>(SelectObject(hMemDC, hDDBmp));
    BitBlt(hMemDC, 0, 0, size.cx, size.cy, hdc, 0, 0, SRCCOPY);
    SelectObject(hMemDC, hOldBmp);
    HBITMAP hBmp = static_cast<HBITMAP>(CopyImage(hDDBmp, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION));
	HPALETTE hPalette = (HPALETTE)GetCurrentObject(hdc, OBJ_PAL);
	Bitmap *myBitMap = new Bitmap(hBmp, hPalette);
	DumpImage(myBitMap);
	delete myBitMap;
	myBitMap = NULL;
    DeleteObject(hDDBmp);
    DeleteDC(hMemDC);
    ReleaseDC(hWnd, hdc);
	DeleteObject(hBmp);
    return TRUE;
}

void DumpImage(Bitmap* aBmp)
{
	CLSID encoderClsid;
	GetEncoderClsid(L"image/png", &encoderClsid);
	aBmp->Save(L"desktop.png", &encoderClsid);
}

BOOL CaptureDesktop()
{
	return true;
}



int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
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