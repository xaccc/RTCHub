#ifndef _WREIWEBRTCRENDERER_H_
#define _WREIWEBRTCRENDERER_H_

#include "JSAPIAuto.h"
#include "PluginWindow.h"

#include "WENRtcCommon.h"

#include "talk/media/base/videorenderer.h"
#include "talk/media/base/videoframe.h"
#include "talk/media/base/videocommon.h"
#include "talk/base/scoped_ptr.h"
#include "talk/app/webrtc/mediastreaminterface.h"


class WebrtcRendererAPI;

namespace irender {

class IVideoRenderer : public webrtc::VideoRendererInterface,
    public talk_base::RefCountInterface,
    public FB::JSAPIAuto
{
public:
    virtual ~IVideoRenderer() {}

    virtual bool Init() = 0;
    virtual void Deinit() = 0;
    virtual void RedrawRenderArea() = 0;

    virtual void setRendererAPI(WebrtcRendererAPI *ptr) = 0;
    virtual void setPluginWindow(FB::PluginWindow *window) = 0;
    virtual void setWindowless(bool windowless) = 0;
};

class VideoRenderer : public IVideoRenderer 
{
public:
    VideoRenderer(const std::string label, int width, int height);
    virtual ~VideoRenderer();

    virtual void SetSize(int width, int height);
    virtual void RenderFrame(const cricket::VideoFrame* pFrame);

    virtual void setRendererAPI(WebrtcRendererAPI *ptr);
    virtual void setPluginWindow(FB::PluginWindow *window);
    virtual void setWindowless(bool windowless);

protected:
    boost::mutex m_mutex;
    const std::string & m_label;
    WebrtcRendererAPI * m_renderjs;
    FB::PluginWindow * m_window;
    bool m_windowless;

    int m_width;
    int m_height;
    talk_base::scoped_array<uint8> m_image;
};

bool CreateVideoRenderer(const std::string label, int width, int height, talk_base::scoped_refptr<IVideoRenderer> &);
bool DestroyVideoRenderer(talk_base::scoped_refptr<IVideoRenderer> &);


//
// for renderer API in external interfaces
class IWebrtcRenderer : public FB::JSAPIAuto {
public:
    virtual ~IWebrtcRenderer() {}
    virtual talk_base::scoped_refptr<IVideoRenderer> getRenderer() = 0;
    virtual void setRendererWindow(FB::PluginWindow * window) = 0;
};

}

#endif

