#include "WENVideoRenderer.h"
#include "rtc_common.h"

namespace irender {

/**
 * for class VideoRenderer
 */
bool VideoRendererWin::OnRefreshRenderArea(IVideoRenderer *pRender, FB::RefreshEvent *evt, FB::PluginWindow *pWin)
{
    // Only windowless mode, draw render in OnRefreshRenderArea()
    VideoRendererWin* pRenderWin = reinterpret_cast<VideoRendererWin*>(pRender);
    if (pRenderWin) {
        pRenderWin->setPluginWindow(pWin);
        pRenderWin->RedrawRenderArea();
        return true;
    }
    return false;
}

bool VideoRendererWin::Init()
{
    return true;
}

void VideoRendererWin::Deinit()
{
}

void VideoRendererWin::RedrawRenderArea()
{
    boost::mutex::scoped_lock autolock(m_mutex);

    return_assert(m_window);
    return_assert(m_image.get());

    int width = m_window->getWindowWidth();
    int height = m_window->getWindowHeight();

    FB::PluginWindowlessWin* pWin2 = dynamic_cast<FB::PluginWindowlessWin*>(m_window);
    if(NULL != pWin2 && NULL != m_image.get()) {
        LOGD("this is a windowless plugin");
        HDC hdc = pWin2->getHDC();
        FB::Rect rect = m_window->getWindowPosition();

        SetStretchBltMode(hdc, HALFTONE);
        StretchDIBits(hdc, rect.left, rect.bottom, width, -height, 0, 0, m_width, m_height, 
                m_image.get(), &m_bmi, DIB_RGB_COLORS, SRCCOPY);
        return;
    }
}

void VideoRendererWin::SetSize(int width, int height)
{
    VideoRenderer::SetSize(width, height);

    boost::mutex::scoped_lock autolock(m_mutex);
    m_bmi.bmiHeader.biWidth = width;
    m_bmi.bmiHeader.biHeight = height;
    m_bmi.bmiHeader.biSizeImage = width * height * (m_bmi.bmiHeader.biBitCount >> 3);
}

void VideoRendererWin::RenderFrame(const cricket::VideoFrame* pFrame)
{
    VideoRenderer::RenderFrame(pFrame);

    if (m_window) {
        RedrawRenderArea();
        m_window->InvalidateWindow();
    }
}

VideoRendererWin::VideoRendererWin(const std::string label, int width, int height) 
    : VideoRenderer(label, width, height)
{
    ZeroMemory(&m_bmi, sizeof(m_bmi));
    m_bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    m_bmi.bmiHeader.biPlanes = 1;
    m_bmi.bmiHeader.biBitCount = 32;
    m_bmi.bmiHeader.biCompression = BI_RGB;
    m_bmi.bmiHeader.biWidth = width;
    m_bmi.bmiHeader.biHeight = height;
    m_bmi.bmiHeader.biSizeImage = width * height * (m_bmi.bmiHeader.biBitCount >> 3);
}

VideoRendererWin::~VideoRendererWin()
{
    LOGE("destructor for VideoRendererWin");
}

} // namespace irender

