#ifndef _WEN_VIDEORENDERER_X11_H_
#define _WEN_VIDEORENDERER_X11_H_

#include "../WENRtcCommon.h"
#include "../WENIWebrtcRenderer.h"

#include "X11/PluginWindowX11.h"
#include <gtk/gtk.h>

namespace irender
{

class VideoRendererX11 : public VideoRenderer
{
public:
    static gboolean OnRefreshRenderArea(gpointer pData);

public:
    explicit VideoRendererX11(const std::string label, int width, int height);
    virtual ~VideoRendererX11();

    // for IVideoRenderer
    virtual bool Init();
    virtual void Deinit();
    virtual void RedrawRenderArea();

    // for class IVideoRendererJingle
    virtual void SetSize(int width, int height);
    virtual void RenderFrame(const cricket::VideoFrame* pFrame);

protected:

};

} // namespace irender

#endif // _WREVIDEORENDERER_H_

