// HogVideo.h: interface for the HogVideo class.
//
//////////////////////////////////////////////////////////////////////

#ifndef HOGVIDEO_H
#define HOGVIDEO_H

class HogVideo  
{
public:
	HogVideo();
	virtual ~HogVideo();

    void    SetVideo(LPCTSTR szFilename);
    BOOL    Hog();
    BOOL    UnHog();
    void    Cleanup();
    BOOL    IsHogging();

private:
    BOOL    m_HogEnabled;
    CString m_Filename;
    BOOL    m_bCOMInitialized;
    void   *m_pGraph;
    void   *m_pMediaControl;
    void   *m_pEvent;
    void   *m_pVideoWindow;
};

#endif 
