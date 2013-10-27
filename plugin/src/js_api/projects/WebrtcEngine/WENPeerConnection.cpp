#include "WENPeerConnection.h"
#include "WebrtcEngineAPI.h"
#include "WENIWebrtcRenderer.h"

namespace iengine {

RTCPeerConnection::RTCPeerConnection()
{
    m_peerObserver = NULL;

    registerMethod("createOffer",               make_method(this, &RTCPeerConnection::createOffer));
    registerMethod("createAnswer",              make_method(this, &RTCPeerConnection::createAnswer));
    registerMethod("setLocalDescription",       make_method(this, &RTCPeerConnection::setLocalDescription));
    registerProperty("localDescription",        make_property(this,  &RTCPeerConnection::localDescription));
    registerMethod("setRemoteDescription",      make_method(this, &RTCPeerConnection::setRemoteDescription));
    registerProperty("remoteDescription",       make_property(this,  &RTCPeerConnection::remoteDescription));
    registerProperty("readyState",              make_property(this,  &RTCPeerConnection::readyState));

    registerMethod("updateIce",                 make_method(this, &RTCPeerConnection::updateIce));
    registerMethod("addIceCandidate",           make_method(this, &RTCPeerConnection::addIceCandidate));
    registerProperty("iceState",                make_property(this,  &RTCPeerConnection::iceState)); 

    registerProperty("localStreams",            make_property(this,  &RTCPeerConnection::localStreams));
    registerProperty("remoteStreams",           make_property(this,  &RTCPeerConnection::remoteStreams));

    registerMethod("addStream",                 make_method(this, &RTCPeerConnection::addStream));
    registerMethod("removeStream",              make_method(this, &RTCPeerConnection::removeStream));
    registerMethod("close",                     make_method(this, &RTCPeerConnection::close));

    registerMethod("setLocalRenderer",          make_method(this, &RTCPeerConnection::setLocalRenderer));
    registerMethod("setRemoteRenderer",         make_method(this, &RTCPeerConnection::setRemoteRenderer));

    registerMethod("addRenderer",               make_method(this, &RTCPeerConnection::addRenderer));
    registerMethod("removeRenderer",            make_method(this, &RTCPeerConnection::removeRenderer));
}

RTCPeerConnection::~RTCPeerConnection() 
{
    LOGD("begin..");
    close();

    m_localStreams.clear();
    m_remoteStreams.clear();
    m_handlers.clear();
    m_engine = NULL;

    RTCPeerObserver *ptr = m_peerObserver.release();
    if (ptr) 
        delete ptr;
    LOGD("end!");
}

void RTCPeerConnection::close()
{
    m_readyState = kPeerClosed;
    m_iceState = kIceClosed;
}

bool RTCPeerConnection::Init(WebrtcEngineAPI * engine,
        std::map<int, boost::shared_ptr<RTCEventHandler> > & handlers,
        talk_base::scoped_refptr<webrtc::PeerConnectionFactoryInterface> & pcFactory)
{
    LOGD("begin..(RTCPeerConnection)");
    returnv_assert(engine != NULL, false);
    returnv_assert(!handlers.empty(), false);

    m_engine = engine;
    m_handlers = handlers;

    m_peerObserver = new talk_base::RefCountedObject<RTCPeerObserver>();
    if (!m_peerObserver || 
            !m_peerObserver->Init(m_handlers, pcFactory, m_engine->rtcPC(this))) {
        LOGE("Unable to create WebrtcPeerObserver");
        return false;
    }
    LOGD("end");

    return true;
}

void RTCPeerConnection::createOffer(const FB::JSObjectPtr & rtcSdpCallback,
        const FB::JSObjectPtr & rtcErrorCallback, 
        const FB::VariantMap & constraints)
{
    m_handlers[kJSCallbackLabel]->registerJSCallback(kcreateOfferAnswerOk, rtcSdpCallback);
    m_handlers[kJSCallbackLabel]->registerJSCallback(kcreateOfferAnswerError, rtcErrorCallback);
    m_handlers[kActionLabel]->process(kCreateOffer, m_engine->rtcPC(this), FB::variant_list_of(constraints));
}

void RTCPeerConnection::createAnswer(const boost::shared_ptr<RTCSessionDescription> & rtcSdp, 
        const FB::JSObjectPtr & rtcSdpCallback, 
        const FB::JSObjectPtr & rtcErrorCallback, 
        const FB::VariantMap & constraints)
{
    LOGD("begin..");
    m_handlers[kJSCallbackLabel]->registerJSCallback(kcreateOfferAnswerOk, rtcSdpCallback);
    m_handlers[kJSCallbackLabel]->registerJSCallback(kcreateOfferAnswerError, rtcErrorCallback);
    m_handlers[kActionLabel]->process(kCreateAnswer, m_engine->rtcPC(this), FB::variant_list_of(rtcSdp)(constraints));
    LOGD("end");
}

void RTCPeerConnection::setLocalDescription(const boost::shared_ptr<RTCSessionDescription> & rtcSdp, 
        const FB::JSObjectPtr & rtcVoidCallback, 
        const FB::JSObjectPtr & rtcErrorCallback)
{
    LOGD("begin..");
    m_handlers[kJSCallbackLabel]->registerJSCallback(ksetLocalSessionDescriptionVoid, rtcVoidCallback);
    m_handlers[kJSCallbackLabel]->registerJSCallback(ksetLocalSessionDescriptionError, rtcErrorCallback);
    m_handlers[kActionLabel]->process(kSetLocalDescription, m_engine->rtcPC(this), FB::variant_list_of(rtcSdp));
}

boost::shared_ptr<RTCSessionDescription> RTCPeerConnection::localDescription()
{
    LOGD("begin..");
    return m_peerObserver->GetLocalDescription();
}

void RTCPeerConnection::setRemoteDescription(const boost::shared_ptr<RTCSessionDescription> & rtcSdp, 
        const FB::JSObjectPtr & rtcVoidCallback, 
        const FB::JSObjectPtr & rtcErrorCallback)
{
    LOGD("begin..");
    m_handlers[kJSCallbackLabel]->registerJSCallback(ksetRemoteSessionDescriptionVoid, rtcVoidCallback);
    m_handlers[kJSCallbackLabel]->registerJSCallback(ksetRemoteSessionDescriptionError, rtcErrorCallback);
    m_handlers[kActionLabel]->process(kSetRemoteDescription, m_engine->rtcPC(this), FB::variant_list_of(rtcSdp));
    LOGD("end");
}

boost::shared_ptr<RTCSessionDescription> RTCPeerConnection::remoteDescription()
{
    LOGD("begin..");
    return m_peerObserver->GetRemoteDescription();
}

RTCPeerState RTCPeerConnection::readyState()
{
    LOGD("begin..");
    return m_readyState;
}

void RTCPeerConnection::updateIce(const std::string & config, const std::string & constraints, bool restart)
{
    LOGD("begin..");
}

void RTCPeerConnection::addIceCandidate(const boost::shared_ptr<RTCIceCandidate> & iceCandidate)
{
    LOGD("begin..");
    m_handlers[kActionLabel]->process(kAddIceCandidate, m_engine->rtcPC(this), FB::variant_list_of(iceCandidate));
}

RTCIceState RTCPeerConnection::iceState()
{
    LOGD("begin..");
    return m_iceState;
}

std::vector<boost::shared_ptr<MediaStream> > & RTCPeerConnection::localStreams()
{
    LOGD("begin..");
    return m_localStreams;
}

std::vector<boost::shared_ptr<MediaStream> > & RTCPeerConnection::remoteStreams()
{
    LOGD("begin..");
    return m_remoteStreams;
}

void RTCPeerConnection::addStream(const FB::variant & stream, const FB::VariantMap & constraints)
{
    LOGD("begin..(RTCPeerConnection)");
    boost::shared_ptr<MediaStream> mediaStream = stream.convert_cast<boost::shared_ptr<MediaStream> >();
    if (mediaStream.get() && mediaStream->getStream()) {
        LOGD("stream label = "<<mediaStream->label());
        m_handlers[kActionLabel]->process(kAddStream, m_engine->rtcPC(this), FB::variant_list_of(stream)(constraints));
        m_localStreams.push_back(mediaStream);
    }
    LOGD("end");
}

void RTCPeerConnection::removeStream(const FB::variant & stream)
{
    LOGD("begin..");
    boost::shared_ptr<MediaStream> mediaStream = stream.convert_cast<boost::shared_ptr<MediaStream> >();
    if (mediaStream.get() && mediaStream->getStream()) {
        m_handlers[kActionLabel]->process(kRemoveStream, m_engine->rtcPC(this), FB::variant_list_of(stream));
        m_localStreams.clear();
    }
}

void RTCPeerConnection::setLocalRenderer(const unsigned long render, const int action)
{
    LOGD("begin.., render="<<render);
    m_handlers[kActionLabel]->process(kSetLocalRenderer, m_engine->rtcPC(this), FB::variant_list_of(render)(action));
    LOGD("end");
}

void RTCPeerConnection::setRemoteRenderer(const unsigned long render, const int action)
{
    LOGD("begin.., render="<<render);
    m_handlers[kActionLabel]->process(kSetRemoteRenderer, m_engine->rtcPC(this), FB::variant_list_of(render)(action));
    LOGD("end");
}

void RTCPeerConnection::addRenderer(const unsigned long stream, const unsigned long render)
{
    LOGD("begin.., render="<<render);
    m_handlers[kActionLabel]->process(kAddRenderer, m_engine->rtcPC(this), FB::variant_list_of(stream)(render));
    LOGD("end");
}

void RTCPeerConnection::removeRenderer(const unsigned long stream, const unsigned long render)
{
    LOGD("begin.., render="<<render);
    m_handlers[kActionLabel]->process(kRemoveRenderer, m_engine->rtcPC(this), FB::variant_list_of(stream)(render));
    LOGD("end");
}

} // namespace iengine
