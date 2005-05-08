// HogVideo.h: interface for the CHogVideo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HOGVIDEO_H__FC8D302C_587B_424D_9ADC_64FAAB063628__INCLUDED_)
#define AFX_HOGVIDEO_H__FC8D302C_587B_424D_9ADC_64FAAB063628__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <tchar.h>
#include <wtypes.h>

class CHogVideo  
{
public:
	CHogVideo();
	virtual ~CHogVideo();

    void    SetVideo(LPCTSTR szFilename);
    bool    Hog();
    bool    UnHog();
    void    Cleanup();

private:

    wchar_t m_wszFilename[MAX_PATH];
    bool    m_bCOMInitialized;
    void   *m_pGraph;
    void   *m_pMediaControl;
    void   *m_pEvent;
    void   *m_pVideoWindow;
};

#endif // !defined(AFX_HOGVIDEO_H__FC8D302C_587B_424D_9ADC_64FAAB063628__INCLUDED_)
