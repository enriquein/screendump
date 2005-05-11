#pragma once

#ifndef MY_HELPERS
#define MY_HELPERS

#include "stdafx.h"
#include "Windows.h"

void GetNewFilename(char* retVal);

// must pass reference to a char[36] variable or it will shit hair!
void GetNewFilename(char* retVal)
{
	SYSTEMTIME tNow;
	GetSystemTime(&tNow);
	sprintf(retVal, "bSDump-%u-%02u-%02u--%02u-%02u-%02u-%03u", tNow.wYear, tNow.wMonth, tNow.wDay, tNow.wHour, tNow.wMinute, tNow.wSecond, tNow.wMilliseconds);
}
#endif