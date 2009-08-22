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

BOOL WindowCapture::CaptureScreen(const CString& filename)
{
	CSize captureArea(GetSystemMetrics(SM_CXVIRTUALSCREEN), GetSystemMetrics(SM_CYVIRTUALSCREEN));
    return DoCapture(CPoint(0, 0), captureArea, filename);
}

BOOL WindowCapture::CaptureWindow(const CString& filename)
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
	return DoCapture(CPoint(rect.left, rect.top), CSize(rect.Width(), rect.Height()), filename);
}

BOOL WindowCapture::DoCapture(const POINT& coords, const SIZE& areaSize, const CString& filename)
{
	CDC dc;
    HDC hdc = GetDC(NULL);
    dc.Attach(hdc);

    // Create a memory DC into which the bitmap will be captured
    CDC memDC;
    memDC.CreateCompatibleDC(&dc);

    // If there is already a bitmap, delete it as we are going to replace it
    CBitmap bmp;
    bmp.DeleteObject();

    ICONINFO info;
 	GetIconInfo((HICON)::GetCursor(), &info);   
    
    CURSORINFO cursor;
    cursor.cbSize = sizeof(CURSORINFO);
    GetCursorInfo(&cursor);
		
	bmp.CreateCompatibleBitmap(&dc, areaSize.cx, areaSize.cy);
    CBitmap * oldbm = memDC.SelectObject(&bmp);

    // Before we copy the image in, we blank the bitmap to
    // the background fill color
    memDC.FillSolidRect(&CRect(0,0,areaSize.cx, areaSize.cy), RGB(255,255,255));

    // Copy the window image from the window DC into the memory DC
    memDC.BitBlt(0, 0, areaSize.cx, areaSize.cy, &dc, coords.x, coords.y, SRCCOPY|CAPTUREBLT);

    // We need to compensate for pointer position if we're taking a screenshot of a region/window.
    // The super sad news is that: 1) you have to consider all icons as 32x32 as that's a Windows
    // spec thing, 2) THEY ARE NOT TOP ALIGNED. God dammit, was it so hard to make all standard cursors
    // top aligned? That magic "10" you see down here was compensating for the default mouse cursor.
    // I'm even thinking of letting it be adjustable. It's so irritating, I don't even want to think 
    // about doing math or whatever to figure out where the actual tip of the arrow is. 
    CPoint cursorOffset(cursor.ptScreenPos.x - coords.x - 10, cursor.ptScreenPos.y - coords.y - 10);

    // Now draw the image of the cursor that we captured during
    // the mouse move. DrawIcon will draw a cursor as well.
    memDC.DrawIcon(cursorOffset, (HICON)cursor.hCursor);

    memDC.SelectObject(oldbm);	
    
	Bitmap outputBitMap(bmp, NULL);
    
    if(bUseClipboard)
    {
        if(OpenClipboard(NULL))
        {
            EmptyClipboard();
            SetClipboardData(CF_BITMAP, bmp);
            CloseClipboard();
        }
    }
    
    BOOL success = DumpImage(&outputBitMap, filename);

    DeleteObject(bmp.Detach());
    DeleteDC(dc.Detach());
    DeleteDC(memDC.Detach());
    return success;
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

BOOL WindowCapture::DumpImage(Bitmap* aBmp, const CString& filename)
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

    Status stat = aBmp->Save(fullFilePath, &encoderClsid, &eParams);
    return stat == Ok;
}