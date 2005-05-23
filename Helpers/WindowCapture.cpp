#include "stdafx.h"
#include "..\Helpers\Helpers.h"
#include "..\Classes\GlobalSettings.h"
#include "..\Helpers\WindowCapture.h"
#include <io.h>
#include <direct.h>
#include "gdiplus.h"
using namespace Gdiplus;

#define CAPTUREBLT  (DWORD)0x40000000
#define SM_CYVIRTUALSCREEN 79
#define SM_CXVIRTUALSCREEN 78

BOOL CaptureScreen()
{
	int height = GetSystemMetrics(SM_CYVIRTUALSCREEN);
	int width = GetSystemMetrics(SM_CXVIRTUALSCREEN);
	HDC hhdc = GetDC(NULL); // Don't clean up since CaptureRegion destroys it.
	CaptureRegion(hhdc, 0, 0, width, height, TRUE);
    return TRUE;
}

BOOL CaptureWindow()
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
/*	if((GetWindowLong(hWnd, GWL_STYLE) & WS_MAXIMIZE) == WS_MAXIMIZE) // Is window maximized?
	{
		// Modify rect to eliminate mysterious black borders...
		// check if this happens outside of winxp
		rect.bottom += rect.top * 2;
		rect.right += rect.left * 2;
	}*/
	HDC hhdc = GetWindowDC(hWnd); // Don't clean up since CaptureRegion destroys it.
	CaptureRegion(hhdc, 0, 0, rect.Width(), rect.Height(), FALSE);
    return TRUE;
}

BOOL CaptureRegion(HDC &hdc, int xCoord, int yCoord, int iWidth, int iHeight, BOOL bWantOverlayed)
{
	long bltFlags;
	bltFlags = bWantOverlayed ? (CAPTUREBLT|SRCCOPY) : SRCCOPY;
	//HDC hdc = GetDC(NULL);
	HDC hDest = CreateCompatibleDC(hdc);
	HBITMAP hbDesktop = CreateCompatibleBitmap( hdc, iWidth, iHeight ); 
	SelectObject(hDest, hbDesktop);
	BitBlt(hDest, 0,0, iWidth, iHeight, hdc, xCoord, yCoord, bltFlags);
	HPALETTE hPalette = (HPALETTE)GetCurrentObject(hdc, OBJ_PAL);
	Bitmap *myBitMap = new Bitmap(hbDesktop, hPalette);
	DumpImage(myBitMap);
	delete myBitMap;
	myBitMap = NULL;
    DeleteObject(hbDesktop);
	DeleteObject(hPalette);
    DeleteDC(hDest);
	ReleaseDC(NULL, hdc);
	DeleteDC(hdc);
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
	EncoderParameters eParams; // = (EncoderParameters*)malloc(sizeof(EncoderParameters) + sizeof(EncoderParameter));
	CLSID encoderClsid;
	char filename[48];
	char chFilter[60];
	char chDefExt[4];
	CFileDialog* cfSaveAs;
	WCHAR fullpath[_MAX_PATH];
	GetNewFilename(filename);
	gs.ReadSettings();

	// Check if the directory has been deleted:
	if(_access(gs.szOutputDir, 0) == -1)
	{
		if(_mkdir(gs.szOutputDir) != 0)
		{
			MessageBox(NULL, "Unable to find screenshots directory.\nAdditionally, the program tried to create it and failed as well.", "bScreenDumped->DumpImage", MB_OK|MB_ICONERROR);
		}
	}

	switch (gs.sEnc)
	{
	case sEncBMP:
		sprintf(chFilter, "Bitmap Files (*.bmp)|*.bmp|All Files (*.*)|*.*||");
		sprintf(chDefExt, "bmp");
		GetEncoderClsid(L"image/bmp", &encoderClsid);
		sprintf(filename, "%s.bmp", filename);
		eParams.Count = 0;
		break;

	case sEncPNG:
		sprintf(chFilter, "PNG Files (*.png)|*.png|All Files (*.*)|*.*||");
		sprintf(chDefExt, "png");
		GetEncoderClsid(L"image/png", &encoderClsid);
		sprintf(filename, "%s.png", filename);
		eParams.Count = 0;		
		break;

	case sEncJPEG:
		sprintf(chFilter, "JPEG Files (*.jpg)|*.jpg|All Files (*.*)|*.*||");
		sprintf(chDefExt, "jpg");
		GetEncoderClsid(L"image/jpeg", &encoderClsid);
		sprintf(filename, "%s.jpg", filename);
		eParams.Count = 1;
		eParams.Parameter[0].Guid = EncoderQuality;
		eParams.Parameter[0].NumberOfValues = 1;
		eParams.Parameter[0].Type = EncoderParameterValueTypeLong;
		eParams.Parameter[0].Value = &gs.lJpgQuality;
	break;

	default:
		MessageBox(NULL, "Encoder Value not found. This is a very unusual error indeed.", "bScreenDumped->OptionsDialog()", MB_OK | MB_ICONWARNING);
		return;
	}

	if(!(gs.bAutoName))
	{
		ToggleTrayMenu(FALSE);
		cfSaveAs = new CFileDialog(FALSE, chDefExt, filename, OFN_HIDEREADONLY|OFN_EXPLORER, chFilter, NULL);
		if( cfSaveAs->DoModal() == IDOK )
		{
			swprintf(fullpath, L"%S", cfSaveAs->GetPathName());
		}
		delete cfSaveAs;
		ToggleTrayMenu(TRUE);
	}
	else
	{
		swprintf(fullpath, L"%S\\%S", gs.szOutputDir, filename);
	}
	aBmp->Save(fullpath, &encoderClsid, &eParams);
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