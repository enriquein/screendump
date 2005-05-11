#pragma once

#ifndef WINDOWCAPTURE_H
#define WINDOWCAPTURE_H

#include "GlobalSettings.h"
#include "gdiplus.h"
using namespace Gdiplus;

BOOL CaptureScreen();
BOOL CaptureWindow();
int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
void DumpImage(Bitmap* aBmp);

#endif  WINDOWCAPTURE_H