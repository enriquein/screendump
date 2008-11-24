// HogVideo.cpp: implementation of the CHogVideo class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include ".\HogVideo.h"
#include <dshow.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHogVideo::CHogVideo()
{
    m_HogEnabled = false;
    m_bCOMInitialized   = false;
    m_pGraph            = NULL;
    m_pMediaControl     = NULL;
    m_pEvent            = NULL;
    m_pVideoWindow      = NULL;

    HRESULT hr = NULL;

    // Initialise COM.
    hr = CoInitialize(NULL);
    if (FAILED(hr))
    {
        Cleanup();
        return;
    }
    else
        m_bCOMInitialized = true;

    // Create the filter graph manager and query for interfaces.
    hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, 
                            IID_IGraphBuilder, (void **)&m_pGraph);
    if (FAILED(hr))
    {
        Cleanup();
        return;
    }

    IGraphBuilder *pGraph = (IGraphBuilder*)m_pGraph;
    hr = pGraph->QueryInterface(IID_IMediaControl, (void **)&m_pMediaControl);
    if (FAILED(hr))
    {
        Cleanup();
        return;
    }

    hr = pGraph->QueryInterface(IID_IVideoWindow, (void**)&m_pVideoWindow);
    if (FAILED(hr))
    {
        Cleanup();
        return;
    }

    hr = pGraph->QueryInterface(IID_IMediaEvent, (void **)&m_pEvent);
    if (FAILED(hr))
    {
        Cleanup();
        return;
    }
}

CHogVideo::~CHogVideo()
{
    // Wait for completion.
    this->UnHog();

    if (m_pEvent)
    {
        long evCode;
        IMediaEvent *pEvent = (IMediaEvent*)m_pEvent;
        pEvent->WaitForCompletion(INFINITE, &evCode);
    }

    // Clean up.
    this->Cleanup();
}

void CHogVideo::Cleanup()
{
    this->UnHog();

    if (m_pEvent)
    {
        IMediaEvent *pEvent = (IMediaEvent*)m_pEvent;
        pEvent->Release();
        m_pEvent = NULL;
    }

    if (m_pVideoWindow)
    {
        IVideoWindow *pVideoWindow = (IVideoWindow*)m_pVideoWindow;
        pVideoWindow->Release();
        m_pVideoWindow = NULL;
    }

    if (m_pMediaControl)
    {
        IMediaControl *pMediaControl = (IMediaControl*)m_pMediaControl;
        pMediaControl->Release();
        m_pMediaControl = NULL;
    }

    if (m_pGraph)
    {
        IGraphBuilder *pGraph = (IGraphBuilder*)m_pGraph;
        pGraph->Release();
        m_pGraph = NULL;
    }

    if (m_bCOMInitialized)
    {
        CoUninitialize();
        m_bCOMInitialized = false;
    }
}

void CHogVideo::SetVideo(LPCTSTR szFilename)
{
    m_Filename = szFilename;
}

BOOL CHogVideo::Hog()
{
    if (!m_pGraph && !m_pMediaControl && !m_pVideoWindow)
    {
        m_HogEnabled = false;
        return false;
    }

    this->UnHog();

    HRESULT hr = NULL;
    IGraphBuilder *pGraph           = (IGraphBuilder*)  m_pGraph;
    IMediaControl *pMediaControl    = (IMediaControl*)  m_pMediaControl;
    IVideoWindow  *pVideoWindow     = (IVideoWindow*)   m_pVideoWindow;

    // Build the graph. IMPORTANT: Change string to a file on your system.
    hr = pGraph->RenderFile(m_Filename, NULL);
    if (SUCCEEDED(hr))
    {
        // Run the graph.
		pVideoWindow->put_AutoShow(OAFALSE);
        pVideoWindow->put_Visible(OAFALSE);
		pVideoWindow->put_Top(-100);
        pVideoWindow->put_Left(-100);
        pVideoWindow->put_Width(0);
        pVideoWindow->put_Height(0);
        hr = pMediaControl->Run();
        // Hide the window
        pVideoWindow->put_WindowState(SW_HIDE);
        pVideoWindow->put_AutoShow(OAFALSE);
        pVideoWindow->put_Visible(OAFALSE);
        pVideoWindow->put_Top(-100);
        pVideoWindow->put_Left(-100);
        pVideoWindow->put_Width(0);
        pVideoWindow->put_Height(0);

        if (SUCCEEDED(hr))
        {
            // Hog the resource.
            pMediaControl->Pause();
            m_HogEnabled = true;
            return true;
        }
        else
        {
            m_HogEnabled = false;
            return false;
        }
    }
    else
    {
        m_HogEnabled = false;
        return false;
    }
}

BOOL CHogVideo::UnHog()
{
    m_HogEnabled = false;
    if (!m_pMediaControl)
        return false;

    IMediaControl *pMediaControl = (IMediaControl*)m_pMediaControl;
    // Stop the graph.
    pMediaControl->Stop();

    return true;
}

BOOL CHogVideo::IsHogging()
{
	return m_HogEnabled;
}