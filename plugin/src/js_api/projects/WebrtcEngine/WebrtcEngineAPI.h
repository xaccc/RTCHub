/**********************************************************\

  Auto-generated WebrtcEngineAPI.h

\**********************************************************/

#include <string>
#include <sstream>
#include <boost/weak_ptr.hpp>
#include "JSAPIAuto.h"
#include "BrowserHost.h"
#include "WebrtcEngine.h"

#include "WENPeerConnection.h"
#include "WENRtcSdp.h"
#include "WENMediaStream.h"
#include "WENEventHandler.h"
#include "WENRtcCommon.h"

#ifndef H_WebrtcEngineAPI
#define H_WebrtcEngineAPI

class WebrtcEngineAPI : public FB::JSAPIAuto
{
public:
    ////////////////////////////////////////////////////////////////////////////
    /// @fn WebrtcEngineAPI::WebrtcEngineAPI(const WebrtcEnginePtr& plugin, const FB::BrowserHostPtr host)
    ///
    /// @brief  Constructor for your JSAPI object.
    ///         You should register your methods, properties, and events
    ///         that should be accessible to Javascript from here.
    ///
    /// @see FB::JSAPIAuto::registerMethod
    /// @see FB::JSAPIAuto::registerProperty
    /// @see FB::JSAPIAuto::registerEvent
    ////////////////////////////////////////////////////////////////////////////
    WebrtcEngineAPI(const WebrtcEnginePtr& plugin, const FB::BrowserHostPtr& host) :
        m_plugin(plugin), m_host(host)
    {
        LOGD("begin..");

        registerMethod("echo",      make_method(this, &WebrtcEngineAPI::echo));
        registerMethod("testEvent", make_method(this, &WebrtcEngineAPI::testEvent));
        
        // Read-write property
        registerProperty("testString",
                         make_property(this,
                                       &WebrtcEngineAPI::get_testString,
                                       &WebrtcEngineAPI::set_testString));
        
        // Read-only property
        registerProperty("version",
                         make_property(this,
                                       &WebrtcEngineAPI::get_version));

        m_init = false;
        registerMethod("getUserMedia",                  make_method(this, &WebrtcEngineAPI::getUserMedia));
        registerMethod("releaseUserMedia",              make_method(this, &WebrtcEngineAPI::releaseUserMedia));
        registerMethod("getVideoDevices",               make_method(this, &WebrtcEngineAPI::getVideoDevices));
        registerMethod("getAudioDevices",               make_method(this, &WebrtcEngineAPI::getAudioDevices));
        registerMethod("setVideoDevice",                make_method(this, &WebrtcEngineAPI::setVideoDevice));

        registerMethod("createRTCPeerConnection",       make_method(this, &WebrtcEngineAPI::createRTCPeerConnection));
        registerMethod("destroyRTCPeerConnection",      make_method(this, &WebrtcEngineAPI::destroyRTCPeerConnection));
        registerMethod("createRTCSessionDescription",   make_method(this, &WebrtcEngineAPI::createRTCSessionDescription));
        registerMethod("createRTCIceCandidate",         make_method(this, &WebrtcEngineAPI::createRTCIceCandidate));
    }

    ///////////////////////////////////////////////////////////////////////////////
    /// @fn WebrtcEngineAPI::~WebrtcEngineAPI()
    ///
    /// @brief  Destructor.  Remember that this object will not be released until
    ///         the browser is done with it; this will almost definitely be after
    ///         the plugin is released.
    ///////////////////////////////////////////////////////////////////////////////
    virtual ~WebrtcEngineAPI();

    WebrtcEnginePtr getPlugin();

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

private:
    WebrtcEngineWeakPtr m_plugin;
    FB::BrowserHostPtr m_host;

    std::string m_testString;

public:
    // for getUserMedia JS interfaces
    void getUserMedia(const FB::VariantMap & constraints,
            const FB::JSObjectPtr & getUserMediaOkCallback,
            const FB::JSObjectPtr & getUserMediaFailedCallback);
    void releaseUserMedia(boost::shared_ptr<iengine::MediaStream> stream);

    // for audio/video devices
    FB::VariantMap getVideoDevices();
    FB::VariantList getAudioDevices(bool bInput);
    void setVideoDevice(std::string unique_id);

    // JS interfaces for pc/sdp/ice
    boost::shared_ptr<iengine::RTCPeerConnection> createRTCPeerConnection();
    void destroyRTCPeerConnection(boost::shared_ptr<iengine::RTCPeerConnection> pc);
    boost::shared_ptr<iengine::RTCSessionDescription> createRTCSessionDescription();
    boost::shared_ptr<iengine::RTCIceCandidate> createRTCIceCandidate();

    // for pc manage
    boost::shared_ptr<iengine::RTCPeerConnection> rtcPC(iengine::RTCPeerConnection *ptr);
    void delPC(iengine::RTCPeerConnection *ptr);

private:
    bool Init_();
    void getUserMediaImpl();

    // for getUserMedia
    bool m_init;
    boost::shared_ptr<iengine::MediaStream> m_mediaStream; // store getUserMedia stream
    std::string m_vid_uid; // selected video device unique id
    FB::VariantMap m_constraints; // store getUserMedia constraints

    // for pc
    std::vector<boost::shared_ptr<iengine::RTCPeerConnection> > m_pcList;
    talk_base::scoped_refptr<webrtc::PeerConnectionFactoryInterface> m_pcFactory;
    std::map<int, boost::shared_ptr<iengine::RTCEventHandler> > m_handlers;
};

#endif // H_WebrtcEngineAPI

