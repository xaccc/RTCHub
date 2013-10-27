/**********************************************************\

  Auto-generated WebrtcRendererAPI.cpp

\**********************************************************/

#include "JSObject.h"
#include "variant_list.h"
#include "DOM/Document.h"
#include "global/config.h"

#include "WebrtcRendererAPI.h"

///////////////////////////////////////////////////////////////////////////////
/// @fn FB::variant WebrtcRendererAPI::echo(const FB::variant& msg)
///
/// @brief  Echos whatever is passed from Javascript.
///         Go ahead and change it. See what happens!
///////////////////////////////////////////////////////////////////////////////
FB::variant WebrtcRendererAPI::echo(const FB::variant& msg)
{
    static int n(0);
    fire_echo("So far, you clicked this many times: ", n++);

    // return "foobar";
    return msg;
}

// Read/Write property testString
std::string WebrtcRendererAPI::get_testString()
{
    return m_testString;
}

void WebrtcRendererAPI::set_testString(const std::string& val)
{
    m_testString = val;
}

// Read-only property version
std::string WebrtcRendererAPI::get_version()
{
    return FBSTRING_PLUGIN_VERSION;
}

void WebrtcRendererAPI::testEvent()
{
    fire_test();
}

const FB::JSAPIPtr WebrtcRendererAPI::get_self()
{
    return shared_from_this();
}

const FB::JSAPIPtr WebrtcRendererAPI::GetSelf()
{
    return shared_from_this();
}

talk_base::scoped_refptr<irender::IVideoRenderer> WebrtcRendererAPI::getRenderer() 
{
    boost::mutex::scoped_lock autolock(m_mutex);

    return m_render;
}

unsigned long WebrtcRendererAPI::getRendererX()
{
    boost::mutex::scoped_lock autolock(m_mutex);

    return (unsigned long)(m_render.get());
}

bool WebrtcRendererAPI::initRenderer(const std::string & label, int width, int height) 
{
    boost::mutex::scoped_lock autolock(m_mutex);

    returnv_assert(!m_init, true);

    LOGD("begin.., m_init="<<m_init);
    if(CreateVideoRenderer(label,  width, height, m_render)) {
        m_render->setRendererAPI(this);
        m_render->setPluginWindow(m_window);
        m_render->setWindowless(m_windowless);
        m_init = true;
        LOGD("render="<<m_render.get()<<", render(long)=" <<(unsigned long)m_render.get());
        return true;
    }
    return false;
}

void WebrtcRendererAPI::setRendererWindow(FB::PluginWindow * window)
{
    boost::mutex::scoped_lock autolock(m_mutex);

    m_window = window;
    if (m_render.get()) {
        m_render->setPluginWindow(window);
    }
}

WebrtcRendererAPI::~WebrtcRendererAPI() 
{ 
    LOGD("begin.."); 
    boost::mutex::scoped_lock autolock(m_mutex);
    m_render.release();
    m_render = NULL;
    LOGD("end!");
}

