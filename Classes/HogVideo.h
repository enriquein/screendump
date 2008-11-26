// HogVideo.h: interface for the HogVideo class.
//
//////////////////////////////////////////////////////////////////////

#ifndef HOGVIDEO_H
#define HOGVIDEO_H
#include <dshow.h>
class HogVideo  
{
public:
	HogVideo();
	virtual ~HogVideo();

    void    SetVideo(const CString& szFilename);
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

// inline defs
inline BOOL HogVideo::UnHog()
{
    m_HogEnabled = false;
    if (!m_pMediaControl)
        return false;

    IMediaControl *pMediaControl = (IMediaControl*)m_pMediaControl;
    // Stop the graph.
    pMediaControl->Stop();

    return true;
}

inline BOOL HogVideo::IsHogging()
{
	return m_HogEnabled;
}

inline void HogVideo::SetVideo(const CString& szFilename)
{
    m_Filename = szFilename;
}
#endif 
