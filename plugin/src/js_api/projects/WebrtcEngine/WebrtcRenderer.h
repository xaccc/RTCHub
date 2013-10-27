/**********************************************************\

  Auto-generated WebrtcRenderer.h

  This file contains the auto-generated main plugin object
  implementation for the WebrtcRenderer project

\**********************************************************/
#ifndef H_WebrtcRendererPLUGIN
#define H_WebrtcRendererPLUGIN

#include <boost/thread.hpp>

#include "PluginWindow.h"
#include "PluginEvents/MouseEvents.h"
#include "PluginEvents/AttachedEvent.h"
#include "PluginEvents/DrawingEvents.h"

#include "PluginCore.h"

#include "WENIWebrtcRenderer.h"

FB_FORWARD_PTR(WebrtcRenderer)
class WebrtcRenderer : public FB::PluginCore
{
public:
    static void StaticInitialize();
    static void StaticDeinitialize();

public:
    WebrtcRenderer(const std::string& mimetype);
    virtual ~WebrtcRenderer();

public:
    void onPluginReady();
    void shutdown();
    virtual FB::JSAPIPtr createJSAPI();
    // If you want your plugin to always be windowless, set this to true
    // If you want your plugin to be optionally windowless based on the
    // value of the "windowless" param tag, remove this method or return
    // FB::PluginCore::isWindowless()
#if defined(IRENDER_WIN)
    virtual bool isWindowless() { return true; }
#else
    virtual bool isWindowless() { return false; }
#endif	

    BEGIN_PLUGIN_EVENT_MAP()
        EVENTTYPE_CASE(FB::MouseDownEvent, onMouseDown, FB::PluginWindow)
        EVENTTYPE_CASE(FB::MouseUpEvent, onMouseUp, FB::PluginWindow)
        EVENTTYPE_CASE(FB::MouseMoveEvent, onMouseMove, FB::PluginWindow)
        EVENTTYPE_CASE(FB::MouseMoveEvent, onMouseMove, FB::PluginWindow)
        EVENTTYPE_CASE(FB::AttachedEvent, onWindowAttached, FB::PluginWindow)
        EVENTTYPE_CASE(FB::DetachedEvent, onWindowDetached, FB::PluginWindow)
        EVENTTYPE_CASE(FB::ResizedEvent, onWindowResized, FB::PluginWindow)
        EVENTTYPE_CASE(FB::RefreshEvent, onWindowRefresh, FB::PluginWindow)
    END_PLUGIN_EVENT_MAP()

    /** BEGIN EVENTDEF -- DON'T CHANGE THIS LINE **/
    virtual bool onMouseDown(FB::MouseDownEvent *evt, FB::PluginWindow *);
    virtual bool onMouseUp(FB::MouseUpEvent *evt, FB::PluginWindow *);
    virtual bool onMouseMove(FB::MouseMoveEvent *evt, FB::PluginWindow *);
    virtual bool onWindowAttached(FB::AttachedEvent *evt, FB::PluginWindow *);
    virtual bool onWindowDetached(FB::DetachedEvent *evt, FB::PluginWindow *);
    virtual bool onWindowResized(FB::ResizedEvent *evt, FB::PluginWindow *);
    virtual bool onWindowRefresh(FB::RefreshEvent *evt, FB::PluginWindow *);
    /** END EVENTDEF -- DON'T CHANGE THIS LINE **/

private:
    std::string  m_mimetype;
    boost::shared_ptr<irender::IWebrtcRenderer> m_render;
};


#endif

