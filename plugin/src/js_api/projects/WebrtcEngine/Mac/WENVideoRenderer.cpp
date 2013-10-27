#include <assert.h>

#include "WENVideoRenderer.h"
#include "rtc_common.h"


namespace irender {

/**
 * for class VideoRenderer
 */
bool VideoRendererMac::OnRefreshRenderArea(IVideoRenderer *pRender, FB::RefreshEvent *pEvt, FB::PluginWindow *pWin)
{
    VideoRendererMac* pRenderMac = reinterpret_cast<VideoRendererMac*>(pRender);
    if (pRenderMac) {
        pRenderMac->SetRefreshEvent(pEvt);
        pRenderMac->setPluginWindow(pWin);
        pRenderMac->RedrawRenderArea();
    }
    return true;
}

void VideoRendererMac::SetRefreshEvent(FB::RefreshEvent *event)
{
    boost::mutex::scoped_lock autolock(m_mutex);
    m_event = event;
}

bool VideoRendererMac::Init()
{
    return true;
}

void VideoRendererMac::Deinit()
{
}

void VideoRendererMac::RedrawRenderArea()
{
    FB::CoreGraphicsDraw* pCgDrawEvt(static_cast<FB::CoreGraphicsDraw*>(m_event));
    CGContextRef pContext = pCgDrawEvt->context;

    boost::mutex::scoped_lock autolock(m_mutex);

    int stride = m_width*4;
    int frameBufferSize = m_height*stride;

    static SInt32 osMajorVersion = 0;
    static SInt32 osMinorVersion = 0;
    static CGInterpolationQuality interpolationMode = kCGInterpolationNone;

    if(0 == osMajorVersion || 0 == osMinorVersion)
    {
        if(noErr != Gestalt(gestaltSystemVersionMajor, &osMajorVersion))
        {
            osMajorVersion = 10;
        }
        if(noErr != Gestalt(gestaltSystemVersionMinor, &osMinorVersion))
        {
            osMinorVersion = 6;
        }
        if(10 <= osMajorVersion && 7 <= osMinorVersion)
        {
            interpolationMode = kCGInterpolationDefault;
        }
    }

    if(NULL == pContext || NULL == m_image.get())
    {
        return false;
    }

    int winWidth = pCgDrawEvt->bounds.right - pCgDrawEvt->bounds.left;
    int winHeight = pCgDrawEvt->bounds.bottom - pCgDrawEvt->bounds.top;

    if(winWidth<=1 || winHeight<=1) {
        return;
    }

    CGContextSaveGState(pContext);
    CGContextSetShouldAntialias(pContext, true);
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    CGImageRef cgImage = CGImageCreate(m_width, m_height, 8, 32, stride, colorSpace,
            kCGImageAlphaNoneSkipLast,
            CGDataProviderCreateWithData(NULL, m_image.get(), frameBufferSize, NULL),
            NULL, false, kCGRenderingIntentDefault);
    if(NULL == cgImage)
    {
        CGColorSpaceRelease(colorSpace);
        CGContextRestoreGState(pContext);
        return;
    }

    CGContextSetInterpolationQuality(pContext, interpolationMode);
    CGContextTranslateCTM(pContext, 0, winHeight);
    CGContextScaleCTM(pContext, 1, -1);
    CGContextDrawImage(pContext, CGRectMake(0, 0, winWidth, winHeight), cgImage);

    CGImageRelease(cgImage);
    CGColorSpaceRelease(colorSpace);
    CGContextRestoreGState(pContext);
    return;
}

void VideoRendererMac::SetSize(int width, int height)
{
     VideoRenderer::SetSize(width, height);
}

void VideoRendererMac::RenderFrame(const cricket::VideoFrame* pFrame)
{
    VideoRenderer::RenderFrame(pFrame);
}

VideoRendererMac::VideoRendererMac(const std::string label, int width, int height) 
    : VideoRenderer(label, width, height)
{
    m_event = NULL;
}

VideoRendererMac::~VideoRendererMac()
{
    LOGD("destructor for VideoRendererMac");
}

} // namespace irender

