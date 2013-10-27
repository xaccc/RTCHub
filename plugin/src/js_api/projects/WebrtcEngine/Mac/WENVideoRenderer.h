#ifndef _WREVIDEORENDERER_H_
#define _WREVIDEORENDERER_H_

#include "../WENRtcCommon.h"
#include "../WENIWebrtcRenderer.h"

#include "Mac/PluginWindowMac.h"
#include "PluginEvents/DrawingEvents.h"

namespace irender
{

class VideoRendererMac : public VideoRenderer
{
public:
    static bool OnRefreshRenderArea(IVideoRenderer *pRender, FB::RefreshEvent *evt, FB::PluginWindow *pWin);

public:
    explicit VideoRendererMac(const std::string label, int width, int height);
    virtual ~VideoRendererMac();

    // for IVideoRenderer
    virtual bool Init();
    virtual void Deinit();
    virtual void RedrawRenderArea();

    // for class IVideoRendererJingle
    virtual void SetSize(int width, int height);
    virtual void RenderFrame(const cricket::VideoFrame* pFrame);

    void SetRefreshEvent(FB::RefreshEvent *event);

protected:
    FB::RefreshEvent * m_event;
};

} // namespace irender

#endif // _WREVIDEORENDERER_H_

