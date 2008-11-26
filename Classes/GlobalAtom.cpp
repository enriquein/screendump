#include "stdafx.h"
#include "Windows.h"
#include ".\globalatom.h"

GlobalAtom::GlobalAtom(void)
{
	SYSTEMTIME sTime;
	GetLocalTime(&sTime);
    CString s;
    s.Format(_T("%u%u%03u"), sTime.wMinute, sTime.wSecond, sTime.wMilliseconds);
	m_nID =	GlobalAddAtom(s);
}

GlobalAtom::~GlobalAtom(void)
{
	GlobalDeleteAtom(m_nID);
}

