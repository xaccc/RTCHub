#include <gdk/gdk.h>
#include <assert.h>
#include <unistd.h>

#include "WENVideoRenderer.h"
#include "rtc_common.h"


namespace irender {

/**
 * for class VideoRenderer
 */
gboolean VideoRendererX11::OnRefreshRenderArea(gpointer pData)
{
    VideoRendererX11* pRenderX11 = reinterpret_cast<VideoRendererX11*>(pData);
    if(pRenderX11) {
        pRenderX11->RedrawRenderArea();
    }

    return FALSE;
}

bool VideoRendererX11::Init()
{
    return true;
}

void VideoRendererX11::Deinit()
{
}

void VideoRendererX11::RedrawRenderArea()
{
    boost::mutex::scoped_lock autolock(m_mutex);
    
    return_assert(m_window);

    gdk_threads_enter();

    FB::PluginWindowX11* pWin = 
        reinterpret_cast<FB::PluginWindowX11*>(m_window);
    GtkWidget * pWidget = pWin->getWidget();

    if (pWidget && m_image.get()) {
        gdk_draw_rgb_32_image(
                pWidget->window,
                pWidget->style->fg_gc[GTK_STATE_NORMAL],
                0,
                0,
                m_width,
                m_height,
                GDK_RGB_DITHER_MAX,
                m_image.get(),
                m_width*4
                );
    }

    gdk_threads_leave();
}

void VideoRendererX11::SetSize(int width, int height)
{
    gdk_threads_enter();
    VideoRenderer::SetSize(width, height);
    gdk_threads_leave();
}

void VideoRendererX11::RenderFrame(const cricket::VideoFrame* pFrame)
{
    gdk_threads_enter();
    VideoRenderer::RenderFrame(pFrame);
    gdk_threads_leave();

    g_idle_add(VideoRendererX11::OnRefreshRenderArea, this);
}

VideoRendererX11::VideoRendererX11(const std::string label, int width, int height) 
    : VideoRenderer(label, width, height)
{
    //TODO: Hack - wait till resize happens from javascript
    //usleep(1000000);
}

VideoRendererX11::~VideoRendererX11()
{
    LOGD("destructor for VideoRendererX11");
    //TODO: Hack - wait till resize happens from javascript
    //usleep(1000000);
}

} // namespace irender

