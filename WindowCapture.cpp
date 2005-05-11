#include "stdafx.h"
#include "Helpers.h"
#include "GlobalSettings.h"
#include "WindowCapture.h"
#include "gdiplus.h"
using namespace Gdiplus;

#define CAPTUREBLT          (DWORD)0x40000000
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

BOOL CaptureScreen()
{
	HDC hdc = GetDC(NULL);
	HDC hDest = CreateCompatibleDC(hdc);
	//int height = GetSystemMetrics(SM_CYVIRTUALSCREEN);
	//int width = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	int height = GetSystemMetrics(SM_CYSCREEN);
	int width = GetSystemMetrics(SM_CXSCREEN);
	HBITMAP hbDesktop = CreateCompatibleBitmap( hdc, width, height );
	SelectObject(hDest, hbDesktop);
	BitBlt(hDest, 0,0, width, height, hdc, 0, 0, CAPTUREBLT|SRCCOPY);
	HPALETTE hPalette = (HPALETTE)GetCurrentObject(hdc, OBJ_PAL);
	Bitmap *myBitMap = new Bitmap(hbDesktop, hPalette);
	DumpImage(myBitMap);
	delete myBitMap;
	myBitMap = NULL;
    DeleteObject(hbDesktop);
    DeleteDC(hDest);
	ReleaseDC(NULL, hdc);
	DeleteObject(hPalette);
    return TRUE;
}
/*
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
    CRect rect;
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
	rect.NormalizeRect();
	HBITMAP hbWindow = CreateCompatibleBitmap( hdc, rect.Width(), rect.Height() );
	SelectObject(hMemDC, hbWindow);
	BitBlt(hMemDC, 0,0, rect.Width(), rect.Height(), hdc, 0, 0, SRCCOPY);
	HPALETTE hPalette = (HPALETTE)GetCurrentObject(hdc, OBJ_PAL);
	Bitmap *myBitMap = new Bitmap(hbWindow, hPalette);

	DumpImage(myBitMap);
	delete myBitMap;
	myBitMap = NULL;
    DeleteObject(hbWindow);
    DeleteDC(hMemDC);
    ReleaseDC(hWnd, hdc);
    return TRUE;
}

void DumpImage(Bitmap* aBmp)
{
	// MSDN NOTE:
	//When you create an EncoderParameters object, you must allocate enough memory
	//to hold all of the EncoderParameter objects that will eventually be placed in
	//the array. For example, if you want to create an EncoderParameters object that will
	//hold an array of five EncoderParameter objects, you should use code similar to the following:

	//	EncoderParameters* pEncoderParameters = (EncoderParameters*)
	//	malloc(sizeof(EncoderParameters) + 4 * sizeof(EncoderParameter));

	CGlobalSettings gs;
	EncoderParameters* eParams = (EncoderParameters*)malloc(sizeof(EncoderParameters) + sizeof(EncoderParameter));
	CLSID encoderClsid;
	char filename[48];
	WCHAR fullpath[_MAX_PATH];
	GetNewFilename(filename);
	gs.ReadSettings();
	switch (gs.sEnc)
	{
	case sEncBMP:
		GetEncoderClsid(L"image/bmp", &encoderClsid);
		sprintf(filename, "%s.bmp", filename);
		eParams->Count = 0;
		break;

	case sEncPNG:
		GetEncoderClsid(L"image/png", &encoderClsid);
		sprintf(filename, "%s.png", filename);
		eParams->Count = 0;		
		break;

	case sEncJPEG:
		GetEncoderClsid(L"image/jpeg", &encoderClsid);
		sprintf(filename, "%s.jpg", filename);
		eParams->Count = 1;
		eParams->Parameter[0].Guid = EncoderCompression;
		eParams->Parameter[0].NumberOfValues = 1;
		eParams->Parameter[0].Type = EncoderParameterValueTypeLong;
		eParams->Parameter[0].Value = &gs.lJpgQuality;
	break;

	default:
		MessageBox(NULL, "Encoder Value not found. This is a very unusual error indeed.", "bScreenDumped->OptionsDialog()", MB_OK | MB_ICONWARNING);
		free(eParams);
		return;
	}

	// if no autoname pop up the save file dialog

	swprintf(fullpath, L"%S\\%S", gs.szOutputDir, filename);
	aBmp->Save(fullpath, &encoderClsid, eParams);
	free(eParams);
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