// HogVideo.h: interface for the CHogVideo class.
//
//////////////////////////////////////////////////////////////////////

#ifndef HOGVIDEO_H
#define HOGVIDEO_H

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

#endif 
