#ifndef WINDOWCAPTURE_H
#define WINDOWCAPTURE_H
#include "gdiplus.h"
using namespace Gdiplus;

enum selEncoder 
{
	sEncBMP,
	sEncJPEG,
	sEncPNG,
	sEncTIFF // sEncTIFF: Not actually implemented yet
};

class WindowCapture
{
public:
    WindowCapture();
    ~WindowCapture();

    BOOL CaptureRegion(int xCoord, int yCoord, int iWidth, int iHeight, CString strFilename);
    BOOL CaptureScreen(CString strFilename);
    BOOL CaptureWindow(HDC &hdc, CString strFilename);
    void SetEncoder(selEncoder enc, long lQualityVal);
    selEncoder GetEncoder();
    long GetQuality();
    void SetQuality(long lQualityVal);

protected:
    long lQuality;      // encoder quality, so far only useful for jpg.
    selEncoder sEnc;    // encoder type. jpg/png/bmp/tiff (multipage not supported).
	GdiplusStartupInput gdiStartupInput;
	ULONG_PTR gdiToken; // token passed to gdistartup/gdishutdown 

    BOOL StartGDI();
    void StopGDI();
    BOOL DoCapture(HDC &hdc, int xCoord, int yCoord, int iWidth, int iHeight, BOOL bWantOverlayed, CString strFilename);
    int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
    void DumpImage(Bitmap* aBmp, LPCTSTR filename);
};
#endif