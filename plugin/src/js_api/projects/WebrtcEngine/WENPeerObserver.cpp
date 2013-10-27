#include "WENPeerObserver.h"

namespace iengine {

class DummySetSessionDescriptionObserver : public webrtc::SetSessionDescriptionObserver 
{
public:
    static DummySetSessionDescriptionObserver* Create() {
        return new talk_base::RefCountedObject<DummySetSessionDescriptionObserver>();
    }
    virtual void OnSuccess() {
    }
    virtual void OnFailure(const std::string& error) {
    }

protected:
    DummySetSessionDescriptionObserver() {}
    ~DummySetSessionDescriptionObserver() {}
};


/**
 * for class RTCPeerObserver
 */

RTCPeerObserver::RTCPeerObserver()
{}

RTCPeerObserver::~RTCPeerObserver()
{
    LOGD("begin..");
    m_peerConnection.release();
    m_rtcPC.reset();
    m_handlers.clear();
    m_renders.clear();
    LOGD("end!");
}

bool RTCPeerObserver::Init(std::map<int, boost::shared_ptr<RTCEventHandler> > & handlers,
        talk_base::scoped_refptr<webrtc::PeerConnectionFactoryInterface> & pcFactory,
        boost::shared_ptr<RTCPeerConnection> rtcPC)
{
    returnv_assert(rtcPC, false);
    returnv_assert(pcFactory, false);

    LOGD("begin..(RTCPeerObserver)");
    m_handlers = handlers;
    m_rtcPC = rtcPC;

    webrtc::PeerConnectionInterface::IceServers servers;
    webrtc::PeerConnectionInterface::IceServer server;
    server.uri = kDefaultIceServer;
    servers.push_back(server);
    m_peerConnection = pcFactory->CreatePeerConnection(servers, NULL, this);
    if (!m_peerConnection) {
        LOGE("failed to CreatePeerConnection");
        return false;
    }

    LOGD("end");
    return true;
}

void RTCPeerObserver::OnError(void)
{
    LOGD("begin..(RTCPeerObserver)");
}

void RTCPeerObserver::OnRenegotiationNeeded()
{
    LOGD("begin..");
    m_handlers[kEventLabel]->process(konnegotationneeded, m_rtcPC, FB::variant_list_of());
    LOGD("end");
}

void RTCPeerObserver::OnIceCandidate(const webrtc::IceCandidateInterface* candidate)
{
    LOGD("begin..");
    std::string sdp;
    if (!candidate || !candidate->ToString(&sdp)) {
        LOGE("Failed to serialize candidate");
        return;
    }

    LOGD("sdp_mid="<<candidate->sdp_mid()<<", sdp_mline_index="<<candidate->sdp_mline_index()<<", sdp=\n"<<sdp);
    boost::shared_ptr<RTCIceCandidate> iceCandidate = boost::make_shared<RTCIceCandidate>(
            sdp,
            candidate->sdp_mid(),
            candidate->sdp_mline_index());
    m_handlers[kEventLabel]->process(konicecandidate, m_rtcPC, FB::variant_list_of(iceCandidate));
    LOGD("end");
}

void RTCPeerObserver::OnStateChange(webrtc::PeerConnectionObserver::StateType state_changed)
{
    LOGD("begin.., state_changed="<<state_changed);
    m_handlers[kEventLabel]->process(konstatechange, m_rtcPC, FB::variant_list_of((int)state_changed));
    LOGD("end");
}

void RTCPeerObserver::OnIceGatheringChange(webrtc::PeerConnectionInterface::IceGatheringState new_state)
{
    LOGD("begin.., ice_state="<<new_state);
    m_handlers[kEventLabel]->process(konicechange, m_rtcPC, FB::variant_list_of((int)new_state));
    LOGD("end");
}

void RTCPeerObserver::OnAddStream(webrtc::MediaStreamInterface* stream)
{
    LOGD("begin..");
    stream->AddRef();
    // TODO: return this remote stream, and call stream->Release() when not used
#if 0
    boost::shared_ptr<MediaStream> mediaStream = 
        boost::make_shared<MediaStream>("remote stream [add]");
    mediaStream->setStream(stream);
    m_handlers[kEventLabel]->process(konaddstream, m_rtcPC, FB::variant_list_of(mediaStream));
#else
    unsigned long istream = (unsigned long) stream;
    m_handlers[kEventLabel]->process(konaddstream, m_rtcPC, FB::variant_list_of(istream));
#endif
    LOGD("end");
}

void RTCPeerObserver::OnRemoveStream(webrtc::MediaStreamInterface* stream)
{
    LOGD("begin..");
    stream->AddRef();
    // TODO: should remove this remote stream 
#if 0
    boost::shared_ptr<MediaStream> mediaStream = 
        boost::make_shared<MediaStream>("remote stream [remove]");
    mediaStream->setStream(stream);
    m_handlers[kEventLabel]->process(konremovestream, m_rtcPC, FB::variant_list_of(mediaStream));
#else
    unsigned long istream = (unsigned long) stream;
    m_handlers[kEventLabel]->process(konremovestream, m_rtcPC, FB::variant_list_of(istream));
#endif
    LOGD("end");
}

void RTCPeerObserver::OnSuccess(webrtc::SessionDescriptionInterface* desc) 
{
    LOGD("begin..");
    std::string type = desc->type();
    std::string sdp;
    return_assert(desc->ToString(&sdp));

    LOGD("send kcreateOfferAnswerOk");
    boost::shared_ptr<RTCSessionDescription> rtcSdp = 
        boost::make_shared<RTCSessionDescription>(type, sdp);
    m_handlers[kJSCallbackLabel]->processJSCallback(kcreateOfferAnswerOk, FB::variant_list_of(rtcSdp));
    LOGD("end");
}

void RTCPeerObserver::OnFailure(const std::string& error) 
{
    LOGD("begin.., error="<<error);
    m_handlers[kJSCallbackLabel]->processJSCallback(kcreateOfferAnswerError, FB::variant_list_of(error));
    LOGD("end");
}

//
// process request from local and interaction with remote.
// CreateOffer => js callback => js SetLocalDescription => send sdp to remote
void RTCPeerObserver::CreateOffer()
{
    LOGD("begin.. m_peerConnection = "<<m_peerConnection.get());
    m_peerConnection->CreateOffer(this, NULL);
    LOGD("end");
}

boost::shared_ptr<RTCSessionDescription> RTCPeerObserver::GetLocalDescription()
{
    std::string type;
    std::string sdp;
    const webrtc::SessionDescriptionInterface* session_description = m_peerConnection->local_description();
    if (session_description) {
        session_description->ToString(&sdp);
        type = session_description->type();;
    }

    return boost::make_shared<RTCSessionDescription>(type, sdp);
}

void RTCPeerObserver::SetLocalDescription(const boost::shared_ptr<RTCSessionDescription> & rtcSdp)
{
    return_assert(rtcSdp);
    return_assert(!(rtcSdp->type().empty()));

    LOGD("rtcSdp->type="<<rtcSdp->type()<<", sdp=\n"<<rtcSdp->sdp());
    webrtc::SessionDescriptionInterface* session_description(
            webrtc::CreateSessionDescription(rtcSdp->type(), rtcSdp->sdp()));
    if (session_description) {
        m_peerConnection->SetLocalDescription(DummySetSessionDescriptionObserver::Create(), 
                session_description);
    }
    LOGD("end");
}

//
// process request from remote
// recv remote sdp -> js SetRemoteDescription -> CreateAnswer
boost::shared_ptr<RTCSessionDescription> RTCPeerObserver::GetRemoteDescription()
{
    std::string type;
    std::string sdp;
    const webrtc::SessionDescriptionInterface* session_description = m_peerConnection->remote_description();
    if (session_description) {
        session_description->ToString(&sdp);
        type = session_description->type();;
    }
    return boost::make_shared<RTCSessionDescription>(type, sdp);
}

void RTCPeerObserver::SetRemoteDescription(const boost::shared_ptr<RTCSessionDescription> & rtcSdp)
{
    LOGD("begin");
    return_assert(rtcSdp);
    return_assert(!(rtcSdp->type().empty()));

    LOGD("rtcSdp->type="<<rtcSdp->type());
    webrtc::SessionDescriptionInterface* session_description(
            webrtc::CreateSessionDescription(rtcSdp->type(), rtcSdp->sdp()));
    if (session_description) {
        m_peerConnection->SetRemoteDescription(DummySetSessionDescriptionObserver::Create(), 
                session_description);
    }
    LOGD("end");
}

void RTCPeerObserver::CreateAnswer(const boost::shared_ptr<RTCSessionDescription> & rtcSdp)
{
    return_assert(rtcSdp);
    return_assert(!(rtcSdp->type().empty()));

    LOGD("rtcSdp->type="<<rtcSdp->type());
    if (rtcSdp->type() == webrtc::SessionDescriptionInterface::kOffer) {
        m_peerConnection->CreateAnswer(this, NULL);
    }
    LOGD("end");
}

void RTCPeerObserver::AddStream(talk_base::scoped_refptr<webrtc::MediaStreamInterface> stream, 
        const webrtc::MediaConstraintsInterface* constraints)
{
    LOGD("begin..(RTCPeerObserver)"<<", m_peerConnection = "<<m_peerConnection.get());
    m_peerConnection->AddStream(stream, constraints);
    LOGD("end");
}

void RTCPeerObserver::RemoveStream(webrtc::MediaStreamInterface* stream)
{
    LOGD("begin");
    m_peerConnection->RemoveStream(stream);
    LOGD("end");
}

void RTCPeerObserver::AddIceCandidate(const boost::shared_ptr<RTCIceCandidate> &candidate)
{
    LOGD("begin");
    std::string sdp_mid = candidate->sdpMid();
    int sdp_mlineindex = candidate->sdpMLineIndex();
    std::string sdp = candidate->candidate();

    talk_base::scoped_ptr<webrtc::IceCandidateInterface> candidate2(
            webrtc::CreateIceCandidate(sdp_mid, sdp_mlineindex, sdp));
    if (!candidate2.get()) {
        LOGE("Can't parse received candidate message.");
        return;
    }

    if (!m_peerConnection->AddIceCandidate(candidate2.get())) {
        LOGE("Failed to apply the received candidate");
        return;
    }
    LOGD("end");
}

void RTCPeerObserver::SetLocalRenderer(webrtc::VideoRendererInterface *render, int action)
{
    return_assert(render);
    return_assert(m_peerConnection);

    talk_base::scoped_refptr<webrtc::StreamCollectionInterface> streams = m_peerConnection->local_streams();
    if(!streams || streams->count() <= 0) {
        LOGE("no valid local_streams");
        return;
    }

    webrtc::MediaStreamInterface * stream = streams->at(0); 
    webrtc::VideoTrackVector tracks = stream->GetVideoTracks();
    LOGD("stream label="<<stream->label()<<", video tracks num = " << tracks.size());
    if (!tracks.empty()) {
        talk_base::scoped_refptr<webrtc::VideoTrackInterface> track = tracks[0];
        if (action == 0) {
            track->AddRenderer(render);
        }else {
            track->RemoveRenderer(render);
        }
    }
    stream->Release();
}

void RTCPeerObserver::SetRemoteRenderer(webrtc::VideoRendererInterface *render, int action)
{
    return_assert(render);
    return_assert(m_peerConnection);

    talk_base::scoped_refptr<webrtc::StreamCollectionInterface> streams = m_peerConnection->remote_streams();
    if(!streams || streams->count() <= 0) {
        LOGE("no valid remote_streams");
        return;
    }

    webrtc::MediaStreamInterface * stream = streams->at(0); 
    webrtc::VideoTrackVector tracks = stream->GetVideoTracks();
    LOGD("stream label="<<stream->label()<<", video tracks num = " << tracks.size());
    if (!tracks.empty()) {
        talk_base::scoped_refptr<webrtc::VideoTrackInterface> track = tracks[0];
        if (action == 0) {
            track->AddRenderer(render);
        }else {
            track->RemoveRenderer(render);
        }
    }
    stream->Release();
}

void RTCPeerObserver::AddRenderer(webrtc::MediaStreamInterface * stream, irender::IVideoRenderer * render)
{
    do {
        break_assert(stream);
        break_assert(render);
        webrtc::VideoTrackVector tracks = stream->GetVideoTracks();
        LOGD("stream label="<<stream->label()<<", video tracks num = " << tracks.size());
        if (render && !tracks.empty() && tracks[0]) {
            tracks[0]->AddRenderer(render);
            //render->AddRef();
            m_renders.push_back(render);
        }
    }while(0);

    if(stream)  stream->Release();
}

void RTCPeerObserver::RemoveRenderer(webrtc::MediaStreamInterface * stream, irender::IVideoRenderer * render)
{
    do {
        break_assert(render);
        std::vector<talk_base::scoped_refptr<irender::IVideoRenderer> >::iterator iter;
        for (iter = m_renders.begin(); iter != m_renders.end(); iter++) {
            if ((*iter).get() == render) {
                iter = m_renders.erase(iter);
                break;
            }
        }

        break_assert(stream);
        webrtc::VideoTrackVector tracks = stream->GetVideoTracks();
        LOGD("stream label="<<stream->label()<<", video tracks num = " << tracks.size());
        if (render && !tracks.empty() && tracks[0]) {
            tracks[0]->RemoveRenderer(render);
        }
    }while(0);

    if (stream) stream->Release();
}


} // namespace iengine
