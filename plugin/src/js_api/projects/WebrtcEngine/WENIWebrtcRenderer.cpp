#include "WENIWebrtcRenderer.h"
#include "WebrtcRendererAPI.h"

#if defined(IRENDER_X11)
#include "X11/WENVideoRenderer.h"
#define RTCVideoRenderer VideoRendererX11
#elif defined(IRENDER_WIN)
#include "Win/WENVideoRenderer.h"
#define RTCVideoRenderer VideoRendererWin
#elif defined(IRENDER_MAC)
#include "Mac/WENVideoRenderer.h"
#define RTCVideoRenderer VideoRendererMac
#else
#error please select one valid platform (x11, win, mac) by corresonding macro!
#endif

namespace irender {

bool CreateVideoRenderer(const std::string label, int width, int height, talk_base::scoped_refptr<IVideoRenderer> & pRender)
{
    pRender = new talk_base::RefCountedObject<RTCVideoRenderer>(label, width, height);
    if(!pRender->Init()) {
        pRender->Deinit();
        pRender = NULL;
    }
    return (pRender != NULL);
}

bool DestroyVideoRenderer(talk_base::scoped_refptr<IVideoRenderer> & pRender)
{
    if (pRender) {
        pRender->Deinit();
        pRender = NULL;
    }
    return true;
}



VideoRenderer::VideoRenderer(const std::string label, int width, int height)
    :
    m_label(label),
    m_width(width),
    m_height(height),
    m_renderjs(NULL),
    m_window(NULL),
    m_windowless(false)
{
    if (width > 0 && height > 0) {
        m_image.reset(new uint8[m_width*m_height*4]);
    }else {
        m_image.reset(NULL);
    }
}

VideoRenderer::~VideoRenderer()
{
    m_renderjs = NULL;
    m_window = NULL;
    m_image.reset(NULL);
}

void VideoRenderer::setRendererAPI(WebrtcRendererAPI *ptr) 
{
    boost::mutex::scoped_lock autolock(m_mutex);
    m_renderjs = ptr;
}

void VideoRenderer::setPluginWindow(FB::PluginWindow *window) 
{
    boost::mutex::scoped_lock autolock(m_mutex);
    m_window = window;
}

void VideoRenderer::setWindowless(bool windowless)
{
    boost::mutex::scoped_lock autolock(m_mutex);
    m_windowless = windowless;
}

void VideoRenderer::SetSize(int width, int height)
{
    LOGD("width="<<width<<", height="<<height);

    boost::mutex::scoped_lock autolock(m_mutex);

    if (m_width != width || m_height != height) {
        if (m_renderjs)
            m_renderjs->fire_onsize(width, height);
    }

    m_width = width;
    m_height = height;
    m_image.reset(new uint8[width * height * 4]);
}

void VideoRenderer::RenderFrame(const cricket::VideoFrame* pFrame)
{
    boost::mutex::scoped_lock autolock(m_mutex);

    return_assert(pFrame);
    return_assert(m_window);
    return_assert(m_image.get());

    return_assert(m_width == pFrame->GetWidth());
    return_assert(m_height == pFrame->GetHeight());

    int frmBufSize = m_width*m_height*4;
    pFrame->ConvertToRgbBuffer(cricket::FOURCC_ARGB,
            m_image.get(),
            frmBufSize,
            m_width*4
            );

#ifdef IRENDER_X11
    uint8* pBufIter = m_image.get();
    uint8* pBufEnd = pBufIter + frmBufSize;
    while(pBufIter < pBufEnd) {
        pBufIter[3] = pBufIter[0];
        pBufIter[0] = pBufIter[2];
        pBufIter[2] = pBufIter[3];
        pBufIter[3] = 0xff;
        pBufIter += 4;
    }
#endif

}

} //  namespace irender
