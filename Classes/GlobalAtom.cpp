#include "stdafx.h"
#include "Windows.h"
#include ".\globalatom.h"

CGlobalAtom::CGlobalAtom(void)
{
	SYSTEMTIME sTime;
	GetLocalTime(&sTime);
    CString s;
    s.Format(_T("%u%u%03u"), sTime.wMinute, sTime.wSecond, sTime.wMilliseconds);
	m_nID =	GlobalAddAtom(s);
}

CGlobalAtom::~CGlobalAtom(void)
{
	GlobalDeleteAtom(m_nID);
}

ATOM CGlobalAtom::GetID()
{
	return m_nID;
}