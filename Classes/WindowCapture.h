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

//    BOOL CaptureRegion(const int& xCoord, const int& yCoord, const int& iWidth, const int& iHeight, const CString& strFilename); // Captures all windows in the region specified by the parameters.
    BOOL CaptureScreen(const CString& strFilename);  // Captures the entire screen.    
    BOOL CaptureWindow(const CString& strFilename);  // Captures the active window (wether it's foreground or not.) If not the foreground window, it will not capture windows on top of it.
    void SetEncoder(const selEncoder& enc, const long& lQualityVal = 100);  // Set encoder to use. Optionally set the quality (Defaults to 100).
    selEncoder GetEncoder();    // Returns the current selected encoder.
    long GetQuality();   // Get quality value (JPG only). Range is 1 to 100.      
    void SetQuality(const long& lQualityVal);  // Set quality for JPG files. Range is: 1 to 100 where 1 is low quality and 100 is best possible quality.
    void SetUseClipboard(const BOOL& wantClipboard); // TRUE if we want to store data on the clipboard, FALSE otherwise.
    BOOL GetUseClipboard(); // Returns if we're storing data on the clipboard after captures.

protected:
    long lQuality;      // Encoder quality, so far only useful for jpg.
    selEncoder sEnc;    // Encoder type. jpg/png/bmp/tiff (multipage not supported).
	GdiplusStartupInput gdiStartupInput;
	ULONG_PTR gdiToken; // Token passed to gdistartup/gdishutdown 
    BOOL bUseClipboard; // Do we want our captured data to reside on the clipboard for other programs to use? Default: TRUE.

    Status lastGdiStatus;
    BOOL StartGDI();
    void StopGDI();
    BOOL DoCapture(const HDC &hdc, const int& xCoord, const int& yCoord, const int& iWidth, const int& iHeight, const BOOL& bWantOverlayed, const CString& strFilename);
    int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
    void DumpImage(Bitmap *aBmp, const CString& filename);
};

// inline defs
inline BOOL WindowCapture::StartGDI()
{
    lastGdiStatus = GdiplusStartup(&gdiToken, &gdiStartupInput, NULL);
    return (lastGdiStatus == Ok);
}

inline void WindowCapture::StopGDI()
{
    if(lastGdiStatus == Ok)
    {
        GdiplusShutdown(gdiToken);
        lastGdiStatus = Aborted;
    }
}

inline void WindowCapture::SetEncoder(const selEncoder& enc, const long& lQualityVal /* = 100 */)
{
    sEnc = enc;
    lQuality = lQualityVal;
}

inline selEncoder WindowCapture::GetEncoder()
{
    return sEnc;
}

inline long WindowCapture::GetQuality()
{
    return lQuality;
}

inline void WindowCapture::SetQuality(const long& lQualityVal)
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

inline BOOL WindowCapture::GetUseClipboard()
{
    return bUseClipboard;
}

inline void WindowCapture::SetUseClipboard(const BOOL& wantClipboard)
{
    bUseClipboard = wantClipboard;
}

//inline BOOL WindowCapture::CaptureRegion(const int& xCoord, const int& yCoord, const int& iWidth, const int& iHeight, const CString& strFilename)
//{
//    return FALSE; // Not yet implemented.
//}
#endif