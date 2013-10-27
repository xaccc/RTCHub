/**********************************************************\

  Auto-generated WebrtcRendererAPI.h

\**********************************************************/

#include <string>
#include <sstream>
#include <boost/weak_ptr.hpp>
#include "JSAPIAuto.h"
#include "BrowserHost.h"
#include "WebrtcRenderer.h"

#include "WENIWebrtcRenderer.h"
#include "WENRtcCommon.h"

#ifndef H_WebrtcRendererAPI
#define H_WebrtcRendererAPI

class WebrtcRendererAPI : public irender::IWebrtcRenderer
{
public:
    ////////////////////////////////////////////////////////////////////////////
    /// @fn WebrtcRendererAPI::WebrtcRendererAPI(const WebrtcRendererPtr& plugin, const FB::BrowserHostPtr host)
    ///
    /// @brief  Constructor for your JSAPI object.
    ///         You should register your methods, properties, and events
    ///         that should be accessible to Javascript from here.
    ///
    /// @see FB::JSAPIAuto::registerMethod
    /// @see FB::JSAPIAuto::registerProperty
    /// @see FB::JSAPIAuto::registerEvent
    ////////////////////////////////////////////////////////////////////////////
    WebrtcRendererAPI(const FB::BrowserHostPtr& host, bool windowless) : m_host(host), m_windowless(windowless)
    {
        LOGD("begin..");
        m_init = false;
        m_window = NULL;

        // for self prop
        registerProperty("self",            make_property(this, &WebrtcRendererAPI::get_self));
        registerMethod("GetSelf",           make_method(this, &WebrtcRendererAPI::GetSelf));

        // m_render;
        registerMethod("initRenderer",      make_method(this, &WebrtcRendererAPI::initRenderer));
        //registerMethod("getRenderer",       make_method(this, &WebrtcRendererAPI::getRenderer));
        registerMethod("getRendererX",       make_method(this, &WebrtcRendererAPI::getRendererX));

        registerMethod("echo",      make_method(this, &WebrtcRendererAPI::echo));
        registerMethod("testEvent", make_method(this, &WebrtcRendererAPI::testEvent));
        
        // Read-write property
        registerProperty("testString",
                         make_property(this,
                                       &WebrtcRendererAPI::get_testString,
                                       &WebrtcRendererAPI::set_testString));
        
        // Read-only property
        registerProperty("version",
                         make_property(this,
                                       &WebrtcRendererAPI::get_version));
    }

    ///////////////////////////////////////////////////////////////////////////////
    /// @fn WebrtcRendererAPI::~WebrtcRendererAPI()
    ///
    /// @brief  Destructor.  Remember that this object will not be released until
    ///         the browser is done with it; this will almost definitely be after
    ///         the plugin is released.
    ///////////////////////////////////////////////////////////////////////////////
    virtual ~WebrtcRendererAPI();

    // Read/Write property ${PROPERTY.ident}
    std::string get_testString();
    void set_testString(const std::string& val);

    // Read-only property ${PROPERTY.ident}
    std::string get_version();

    // Method echo
    FB::variant echo(const FB::variant& msg);
    
    // Event helpers
    FB_JSAPI_EVENT(test, 0, ());
    FB_JSAPI_EVENT(echo, 2, (const FB::variant&, const int));

    // Method test-event
    void testEvent();

    // Read-only self (property, method)
    const FB::JSAPIPtr get_self();
    const FB::JSAPIPtr GetSelf();

    // for irender::IWebrtcRenderer
    virtual talk_base::scoped_refptr<irender::IVideoRenderer> getRenderer();
    virtual unsigned long getRendererX();
    virtual void setRendererWindow(FB::PluginWindow * window);

    // for jsapi method
    bool initRenderer(const std::string & label, int width, int height);

    // for render size changed
    FB_JSAPI_EVENT(onsize, 2, (const int, const int));

private:
    FB::BrowserHostPtr m_host;

    std::string m_testString;

    bool m_init;
    boost::mutex m_mutex;
    FB::PluginWindow * m_window;
    bool m_windowless;
    talk_base::scoped_refptr<irender::IVideoRenderer> m_render;
};

#endif // H_WebrtcRendererAPI

