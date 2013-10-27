/**********************************************************\

  Auto-generated WebrtcRenderer.cpp

  This file contains the auto-generated main plugin object
  implementation for the WebrtcRenderer project

\**********************************************************/

#include "WebrtcRendererAPI.h"

#include "WebrtcRenderer.h"

#if defined(IRENDER_MAC)
#include "Mac/WENVideoRenderer.h"
#elif defined(IRENDER_WIN)
#include "Win/WENVideoRenderer.h"
#endif

#include "WENRtcCommon.h"


///////////////////////////////////////////////////////////////////////////////
/// @fn WebrtcRenderer::StaticInitialize()
///
/// @brief  Called from PluginFactory::globalPluginInitialize()
///
/// @see FB::FactoryBase::globalPluginInitialize
///////////////////////////////////////////////////////////////////////////////
void WebrtcRenderer::StaticInitialize()
{
    // Place one-time initialization stuff here; As of FireBreath 1.4 this should only
    // be called once per process
}

///////////////////////////////////////////////////////////////////////////////
/// @fn WebrtcRenderer::StaticInitialize()
///
/// @brief  Called from PluginFactory::globalPluginDeinitialize()
///
/// @see FB::FactoryBase::globalPluginDeinitialize
///////////////////////////////////////////////////////////////////////////////
void WebrtcRenderer::StaticDeinitialize()
{
    // Place one-time deinitialization stuff here. As of FireBreath 1.4 this should
    // always be called just before the plugin library is unloaded
}

///////////////////////////////////////////////////////////////////////////////
/// @brief  WebrtcRenderer constructor.  Note that your API is not available
///         at this point, nor the window.  For best results wait to use
///         the JSAPI object until the onPluginReady method is called
///////////////////////////////////////////////////////////////////////////////
WebrtcRenderer::WebrtcRenderer(const std::string& mimetype) : m_mimetype(mimetype)
{
    LOGD("WebrtcRenderer's mimetype="<<m_mimetype);
}

///////////////////////////////////////////////////////////////////////////////
/// @brief  WebrtcRenderer destructor.
///////////////////////////////////////////////////////////////////////////////
WebrtcRenderer::~WebrtcRenderer()
{
    LOGD("begin..");

    // This is optional, but if you reset m_api (the shared_ptr to your JSAPI
    // root object) and tell the host to free the retained JSAPI objects then
    // unless you are holding another shared_ptr reference to your JSAPI object
    // they will be released here.
    releaseRootJSAPI();
    m_host->freeRetainedObjects();
}

void WebrtcRenderer::onPluginReady()
{
    // When this is called, the BrowserHost is attached, the JSAPI object is
    // created, and we are ready to interact with the page and such.  The
    // PluginWindow may or may not have already fire the AttachedEvent at
    // this point.
}

void WebrtcRenderer::shutdown()
{
    // This will be called when it is time for the plugin to shut down;
    // any threads or anything else that may hold a shared_ptr to this
    // object should be released here so that this object can be safely
    // destroyed. This is the last point that shared_from_this and weak_ptr
    // references to this object will be valid
}

///////////////////////////////////////////////////////////////////////////////
/// @brief  Creates an instance of the JSAPI object that provides your main
///         Javascript interface.
///
/// Note that m_host is your BrowserHost and shared_ptr returns a
/// FB::PluginCorePtr, which can be used to provide a
/// boost::weak_ptr<WebrtcRenderer> for your JSAPI class.
///
/// Be very careful where you hold a shared_ptr to your plugin class from,
/// as it could prevent your plugin class from getting destroyed properly.
///////////////////////////////////////////////////////////////////////////////
FB::JSAPIPtr WebrtcRenderer::createJSAPI()
{
    LOGD("begin..");
    // m_host is the BrowserHost
    m_render = boost::make_shared<WebrtcRendererAPI>(m_host, isWindowless());
    return m_render;
}

bool WebrtcRenderer::onMouseDown(FB::MouseDownEvent *evt, FB::PluginWindow *)
{
    //LOGD("begin..");
    //printf("Mouse down at: %d, %d\n", evt->m_x, evt->m_y);
    return false;
}

bool WebrtcRenderer::onMouseUp(FB::MouseUpEvent *evt, FB::PluginWindow *)
{
    //LOGD("begin..");
    //printf("Mouse up at: %d, %d\n", evt->m_x, evt->m_y);
    return false;
}

bool WebrtcRenderer::onMouseMove(FB::MouseMoveEvent *evt, FB::PluginWindow *)
{
    //LOGD("begin..");
    //printf("Mouse move at: %d, %d\n", evt->m_x, evt->m_y);
    return false;
}
bool WebrtcRenderer::onWindowAttached(FB::AttachedEvent *evt, FB::PluginWindow *pWin)
{
    LOGD("begin..");
    // The window is attached; act appropriately
    if (m_render.get()) {
        LOGD("set window="<<pWin);
        m_render->setRendererWindow(pWin);
    }
    return false;
}

bool WebrtcRenderer::onWindowDetached(FB::DetachedEvent *evt, FB::PluginWindow *)
{
    // The window is about to be detached; act appropriately
    LOGD("begin..");
    if (m_render.get()) {
        LOGD("set window to be NULL");
        m_render->setRendererWindow(NULL);
    }
    return false;
}

bool WebrtcRenderer::onWindowResized(FB::ResizedEvent *evt, FB::PluginWindow *pWin)
{
    // The window has been resized; act appropriately
    //LOGD("begin..");
    if (m_render.get()) {
        LOGD("set window="<<pWin);
        m_render->setRendererWindow(pWin);
    }
    return false;
}

bool WebrtcRenderer::onWindowRefresh(FB::RefreshEvent *evt, FB::PluginWindow *pWin)
{
    //LOGD("begin..");
    bool ret = false;
#if defined(IRENDER_MAC)
    if (m_render.get()) {
        ret = irender::VideoRendererMac::OnRefreshRenderArea(m_render->getRenderer().get(), evt, pWin);
    }
#elif defined(IRENDER_WIN)
    if (m_render.get() && isWindowless()) {
        ret = irender::VideoRendererWin::OnRefreshRenderArea(m_render->getRenderer().get(), evt, pWin);
    }
#endif
    return ret;
}

