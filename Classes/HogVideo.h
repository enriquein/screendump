// HogVideo.h: interface for the CHogVideo class.
//
//////////////////////////////////////////////////////////////////////

#ifndef HOGVIDEO_H
#define HOGVIDEO_H

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
    CString m_Filename;
    bool    m_bCOMInitialized;
    void   *m_pGraph;
    void   *m_pMediaControl;
    void   *m_pEvent;
    void   *m_pVideoWindow;
};

#endif 
