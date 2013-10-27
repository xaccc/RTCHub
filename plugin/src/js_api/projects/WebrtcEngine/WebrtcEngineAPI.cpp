/**********************************************************\

  Auto-generated WebrtcEngineAPI.cpp

\**********************************************************/

#include "JSObject.h"
#include "variant_list.h"
#include "DOM/Document.h"
#include "global/config.h"

#include "WebrtcEngineAPI.h"

///////////////////////////////////////////////////////////////////////////////
/// @fn FB::variant WebrtcEngineAPI::echo(const FB::variant& msg)
///
/// @brief  Echos whatever is passed from Javascript.
///         Go ahead and change it. See what happens!
///////////////////////////////////////////////////////////////////////////////
FB::variant WebrtcEngineAPI::echo(const FB::variant& msg)
{
    static int n(0);
    fire_echo("So far, you clicked this many times: ", n++);

    // return "foobar";
    return msg;
}

///////////////////////////////////////////////////////////////////////////////
/// @fn WebrtcEnginePtr WebrtcEngineAPI::getPlugin()
///
/// @brief  Gets a reference to the plugin that was passed in when the object
///         was created.  If the plugin has already been released then this
///         will throw a FB::script_error that will be translated into a
///         javascript exception in the page.
///////////////////////////////////////////////////////////////////////////////
WebrtcEnginePtr WebrtcEngineAPI::getPlugin()
{
    WebrtcEnginePtr plugin(m_plugin.lock());
    if (!plugin) {
        throw FB::script_error("The plugin is invalid");
    }
    return plugin;
}

// Read/Write property testString
std::string WebrtcEngineAPI::get_testString()
{
    return m_testString;
}

void WebrtcEngineAPI::set_testString(const std::string& val)
{
    m_testString = val;
}

// Read-only property version
std::string WebrtcEngineAPI::get_version()
{
    return FBSTRING_PLUGIN_VERSION;
}

void WebrtcEngineAPI::testEvent()
{
    fire_test();
}


/**
 * For js interfaces
 */

bool WebrtcEngineAPI::Init_()
{
    returnv_assert(!m_init, true);

    LOGD("begin..(WebrtcEngineAPI), m_init="<<m_init);
    m_pcFactory = webrtc::CreatePeerConnectionFactory();
    if(!m_pcFactory) {
        LOGE("Unable to create peer connection factory");
        return false;
    }

    m_handlers[iengine::kActionLabel] = boost::make_shared<iengine::RTCEventHandler>(iengine::kActionLabel);
    m_handlers[iengine::kEventLabel] = boost::make_shared<iengine::RTCEventHandler>(iengine::kEventLabel);
    m_handlers[iengine::kJSCallbackLabel] = boost::make_shared<iengine::RTCEventHandler>(iengine::kJSCallbackLabel);

    m_handlers[iengine::kActionLabel]->startThread();
    m_handlers[iengine::kEventLabel]->startThread();
    //m_handlers[iengine::kJSCallbackLabel]->startThread();
    m_init = true;

    LOGD("end");
    return true;
}

void WebrtcEngineAPI::getUserMedia(const FB::VariantMap & constraints,
        const FB::JSObjectPtr & getUserMediaOkCallback,
        const FB::JSObjectPtr & getUserMediaFailedCallback)
{
    LOGD("begin..");
    if(!Init_()) {
        throw FB::script_error("Init_ failed!");
    }

    m_constraints = constraints;
    m_handlers[iengine::kJSCallbackLabel]->registerJSCallback(iengine::kgetUserMediaOk, 
            getUserMediaOkCallback);
    m_handlers[iengine::kJSCallbackLabel]->registerJSCallback(iengine::kgetUserMediaError, 
            getUserMediaFailedCallback);
    // FIXME: must call it sync, cannot in another thread
    getUserMediaImpl();
    LOGD("end");
}

void WebrtcEngineAPI::releaseUserMedia(boost::shared_ptr<iengine::MediaStream> stream)
{
    if (stream == m_mediaStream) {
        m_mediaStream.reset();
    }
}

FB::VariantMap WebrtcEngineAPI::getVideoDevices()
{
    return iengine::MediaStreamTrack::GetVideoDevices();
}

FB::VariantList WebrtcEngineAPI::getAudioDevices(bool bInput)
{
    return iengine::MediaStreamTrack::GetAudioDevices(bInput);
}

void WebrtcEngineAPI::setVideoDevice(std::string unique_id)
{
    m_vid_uid = unique_id;
}

boost::shared_ptr<iengine::RTCPeerConnection> WebrtcEngineAPI::createRTCPeerConnection() 
{
    LOGD("begin..");
    if (!Init_()) {
        throw FB::script_error("Init_ Failed!");
    }

    boost::shared_ptr<iengine::RTCPeerConnection> pc = boost::make_shared<iengine::RTCPeerConnection>();
    m_pcList.push_back(pc);
    if(!pc || !pc->Init(this, m_handlers, m_pcFactory)) {
        delPC(pc.get());
        throw FB::script_error("Failed to create RTCPeerConnection!");
    }
    LOGE("end");
    return pc;
}

void WebrtcEngineAPI::destroyRTCPeerConnection(boost::shared_ptr<iengine::RTCPeerConnection> pc)
{
    if (pc) {
        delPC(pc.get());
    }
}

boost::shared_ptr<iengine::RTCSessionDescription> WebrtcEngineAPI::createRTCSessionDescription()
{
    LOGD("begin..");
    if(!Init_()) {
        throw FB::script_error("Init_ Failed!");
    }
    return boost::make_shared<iengine::RTCSessionDescription>();
}

boost::shared_ptr<iengine::RTCIceCandidate> WebrtcEngineAPI::createRTCIceCandidate()
{
    LOGD("begin..");
    if (!Init_()) {
        throw FB::script_error("Failed to create RTCIceCandidate!");
    }
    return boost::make_shared<iengine::RTCIceCandidate>();
}

void WebrtcEngineAPI::getUserMediaImpl()
{
    LOGD("begin..");
    if (!m_mediaStream.get()) {
        boost::shared_ptr<iengine::MediaStream> mediaStream = 
            boost::make_shared<iengine::MediaStream>(iengine::kStreamLabel);
        if (!mediaStream) {
            LOGE("fail to new MediaStream");
            return;
        }

        talk_base::scoped_refptr<webrtc::MediaStreamInterface> stream = 
            m_pcFactory->CreateLocalMediaStream(iengine::kStreamLabel);
        mediaStream->setStream(stream);

        // for audio
        LOGD("for audio track");
        boost::shared_ptr<iengine::MediaStreamTrackList> audioTrackList = mediaStream->audioTracks();
        if (!audioTrackList.get()) {
            LOGE("audioTrackList is NULL ");
            return;
        }

        boost::shared_ptr<iengine::MediaStreamTrack> audioTrack = 
            boost::make_shared<iengine::MediaStreamTrack>(iengine::kAudioKind, iengine::kAudioLabel);
        if (audioTrack.get() && audioTrack->Init(m_pcFactory)) {
            audioTrackList->add(audioTrack);
            stream->AddTrack((webrtc::AudioTrackInterface *)(audioTrack->getTrack().get()));
        }

        // for video
        LOGD("for video track");
        boost::shared_ptr<iengine::MediaStreamTrackList> videoTrackList = mediaStream->videoTracks();
        if (!videoTrackList.get()) {
            LOGE("videoTrackList is NULL ");
            return;
        }

        boost::shared_ptr<iengine::MediaStreamTrack> videoTrack = 
            boost::make_shared<iengine::MediaStreamTrack>(iengine::kVideoKind, iengine::kVideoLabel);
        if (videoTrack.get() && videoTrack->Init(m_pcFactory, "", "", m_vid_uid)) {
            videoTrackList->add(videoTrack);
            stream->AddTrack((webrtc::VideoTrackInterface *)(videoTrack->getTrack().get()));
        }

        // save stream
        m_mediaStream = mediaStream;
    }else {
        LOGI("stream existed");
        std::string errstr = "stream has been existed";
        m_handlers[iengine::kJSCallbackLabel]->processJSCallback(iengine::kgetUserMediaError, 
                FB::variant_list_of(errstr));
    }

    // callback with this stream from getUserMedia
    m_handlers[iengine::kJSCallbackLabel]->processJSCallback(iengine::kgetUserMediaOk, 
            FB::variant_list_of(m_mediaStream));
}

boost::shared_ptr<iengine::RTCPeerConnection> WebrtcEngineAPI::rtcPC(iengine::RTCPeerConnection *ptr)
{
    std::vector<boost::shared_ptr<iengine::RTCPeerConnection> >::iterator iter;
    for (iter = m_pcList.begin(); iter != m_pcList.end(); iter++) {
        if ((*iter).get() == ptr) {
            return (*iter);
        }
    }

    LOGE("find not RTCPeerConnection");
    boost::shared_ptr<iengine::RTCPeerConnection> pc = boost::make_shared<iengine::RTCPeerConnection>();
    m_pcList.push_back(pc);
    return pc;
}

void WebrtcEngineAPI::delPC(iengine::RTCPeerConnection *ptr) 
{
    std::vector<boost::shared_ptr<iengine::RTCPeerConnection> >::iterator iter;
    for (iter = m_pcList.begin(); iter != m_pcList.end(); iter++) {
        if ((*iter).get() == ptr) {
            iter = m_pcList.erase(iter);
            return;
        }
    }

    LOGE("find no RTCPeerConnection");
    return;
}

WebrtcEngineAPI::~WebrtcEngineAPI() 
{ 
    LOGD("begin.."); 

    LOGD("stop event thread");
    if (m_handlers.find(iengine::kEventLabel) != m_handlers.end()) {
        boost::shared_ptr<WebrtcEngineAPI> ptr;
        m_handlers[iengine::kEventLabel]->process(iengine::kKeyQuit, ptr, FB::variant_list_of());
        m_handlers[iengine::kEventLabel]->stopThread();
    }

    LOGD("stop action thread");
    if (m_handlers.find(iengine::kActionLabel) != m_handlers.end()) {
        boost::shared_ptr<WebrtcEngineAPI> ptr;
        m_handlers[iengine::kActionLabel]->process(iengine::kKeyQuit, ptr, FB::variant_list_of());
        m_handlers[iengine::kActionLabel]->stopThread();
    }

    LOGD("release pc");
    std::vector<boost::shared_ptr<iengine::RTCPeerConnection> >::iterator iter;
    for (iter = m_pcList.begin(); iter != m_pcList.end(); iter++) {
        delete (*iter).get();
        (*iter).reset();
    }
    m_pcList.clear();

    LOGD("release pcFactory");
    m_pcFactory.release();

    LOGD("release localstream");
    if (m_mediaStream.get())
        delete m_mediaStream.get();
    m_mediaStream.reset();
    LOGD("end!");
}

