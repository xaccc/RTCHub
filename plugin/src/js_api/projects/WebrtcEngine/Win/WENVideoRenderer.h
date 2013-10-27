#ifndef _WREVIDEORENDERER_H_
#define _WREVIDEORENDERER_H_

#include "../WENRtcCommon.h"
#include "../WENIWebrtcRenderer.h"

#include "Win/PluginWindowWin.h"
#include "Win/PluginWindowlessWin.h"
#include "PluginEvents/DrawingEvents.h"

namespace irender
{

class VideoRendererWin : public VideoRenderer
{
public:
    static bool OnRefreshRenderArea(IVideoRenderer *pRender, FB::RefreshEvent *evt, FB::PluginWindow *pWin);

public:
    explicit VideoRendererWin(const std::string label, int width, int height);
    virtual ~VideoRendererWin();

    // for IVideoRenderer
    virtual bool Init();
    virtual void Deinit();
    virtual void RedrawRenderArea();

    // for class IVideoRendererJingle
    virtual void SetSize(int width, int height);
    virtual void RenderFrame(const cricket::VideoFrame* pFrame);

protected:
    BITMAPINFO m_bmi;
};

} // namespace irender

#endif // _WREVIDEORENDERER_H_

