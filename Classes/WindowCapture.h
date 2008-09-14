#ifndef WINDOWCAPTURE_H
#define WINDOWCAPTURE_H
#include "gdiplus.h"
using namespace Gdiplus;

enum selEncoder 
{
	sEncBMP,    // BMP (Bitmap): uncompressed format. Huge filesize.
	sEncJPEG,   // JPG: compressed format, uses lossy compression (adjustable).
	sEncPNG,    // PNG (Portable Network Graphics): compressed format, uses lossless compression (highest quality, dramatically smaller filesize than BMP)
	sEncTIFF    // TIFF (Tagged Image Format): Not actually implemented yet
};

class WindowCapture
{
public:
    WindowCapture(); 
    ~WindowCapture();

    BOOL CaptureRegion(int xCoord, int yCoord, int iWidth, int iHeight, CString strFilename); // Captures all windows in the region specified by the parameters.
    BOOL CaptureScreen(CString strFilename);  // Captures the entire screen.    
    BOOL CaptureWindow(CString strFilename);  // Captures the active window (wether it's foreground or not.) If not the foreground window, it will not capture windows on top of it.
    void SetEncoder(selEncoder enc, long lQualityVal = 100);  // Set encoder to use. Optionally set the quality (Defaults to 100).
    selEncoder GetEncoder();    // Returns the current selected encoder.
    long GetQuality();   // Get quality value (JPG only). Range is 1 to 100.      
    void SetQuality(long lQualityVal);  // Set quality for JPG files. Range is: 1 to 100 where 1 is low quality and 100 is best possible quality.

protected:
    long lQuality;      // Encoder quality, so far only useful for jpg.
    selEncoder sEnc;    // Encoder type. jpg/png/bmp/tiff (multipage not supported).
	GdiplusStartupInput gdiStartupInput;
	ULONG_PTR gdiToken; // Token passed to gdistartup/gdishutdown 

    Status lastGdiStatus;
    BOOL StartGDI();
    void StopGDI();
    BOOL DoCapture(HDC &hdc, int xCoord, int yCoord, int iWidth, int iHeight, BOOL bWantOverlayed, CString strFilename);
    int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
    void DumpImage(Bitmap* aBmp, LPCTSTR filename);
};
#endif