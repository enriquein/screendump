#pragma once
#include "gdiplus.h"
#include "GlobalSettings.h"
#include "EncoderParameterTypes.h"
using namespace Gdiplus;

class WindowCapture
{
public:
    WindowCapture(GlobalSettings gs); 
    ~WindowCapture();

    BOOL CaptureScreen(const CString& filename);  // Captures the entire screen.    
    BOOL CaptureWindow(const CString& filename);  // Captures the active window (wether it's foreground or not.) If not the foreground window, it will not capture windows on top of it.
    selEncoder GetEncoder();    // Returns the current selected encoder.
    void UpdateSettings(const GlobalSettings& settings); // Updates the settings for this instance. 

protected:
	GdiplusStartupInput gdiStartupInput;
	ULONG_PTR gdiToken; // Token passed to gdistartup/gdishutdown 
    GlobalSettings programSettings;
    Status lastGdiStatus;
    BOOL StartGDI();
    void StopGDI();
    BOOL DoCapture(const POINT& coords, const SIZE& areaSize, const CString& filename);
    int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
    BOOL DumpImage(Bitmap *aBmp, const CString& filename);
};