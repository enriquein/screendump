#ifndef WINDOWCAPTURE_H
#define WINDOWCAPTURE_H

#include "gdiplus.h"
using namespace Gdiplus;

BOOL CaptureDesktop();
BOOL CaptureWindow();
int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
void DumpImage(Bitmap* aBmp);

#endif  WINDOWCAPTURE_H