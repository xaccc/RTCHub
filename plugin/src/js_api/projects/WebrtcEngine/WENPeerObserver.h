#ifndef _WEN_PEER_OBSERVER_H_
#define _WEN_PEER_OBSERVER_H_

#include "WENRtcCommon.h"
#include "WENMediaStream.h"
#include "WENRtcSdp.h"
#include "WENEventHandler.h"
#include "WENIWebrtcRenderer.h"

namespace iengine {

class RTCPeerObserver : 
    public webrtc::PeerConnectionObserver,
    public webrtc::CreateSessionDescriptionObserver
{
public:
    RTCPeerObserver();
    virtual ~RTCPeerObserver();

    bool Init(std::map<int, boost::shared_ptr<RTCEventHandler> > & handlers,
            talk_base::scoped_refptr<webrtc::PeerConnectionFactoryInterface> & pcFactory,
            boost::shared_ptr<RTCPeerConnection> rtcPC);

public:
    // For webrtc::PeerConnectionObserver
    virtual void OnError(void);
    virtual void OnRenegotiationNeeded();
    virtual void OnIceCandidate(const webrtc::IceCandidateInterface* candidate);
    virtual void OnStateChange(webrtc::PeerConnectionObserver::StateType state_changed);
    virtual void OnIceGatheringChange(webrtc::PeerConnectionInterface::IceGatheringState new_state);
    virtual void OnAddStream(webrtc::MediaStreamInterface* stream);
    virtual void OnRemoveStream(webrtc::MediaStreamInterface* stream);

    // For webrtc::CreateSessionDescriptionObserver
    virtual void OnSuccess(webrtc::SessionDescriptionInterface* desc);
    virtual void OnFailure(const std::string& error);

public:
    // for caller and local stream
    void CreateOffer(); 
    void SetLocalDescription(const boost::shared_ptr<RTCSessionDescription> &rtcSdp);

    // for callee and remote stream
    void SetRemoteDescription(const boost::shared_ptr<RTCSessionDescription> &rtcSdp);
    void AddIceCandidate(const boost::shared_ptr<RTCIceCandidate> &candidate);
    void CreateAnswer(const boost::shared_ptr<RTCSessionDescription> &rtcSdp); 

    // get local/remote description
    boost::shared_ptr<RTCSessionDescription> GetLocalDescription();
    boost::shared_ptr<RTCSessionDescription> GetRemoteDescription();

    // add/remove local stream
    void AddStream(talk_base::scoped_refptr<webrtc::MediaStreamInterface> stream, const webrtc::MediaConstraintsInterface* constraints);
    void RemoveStream(webrtc::MediaStreamInterface* stream);

    // add/remove local/remote render
    void SetLocalRenderer(webrtc::VideoRendererInterface *render, int action);
    void SetRemoteRenderer(webrtc::VideoRendererInterface *render, int action);
    void AddRenderer(webrtc::MediaStreamInterface * stream, irender::IVideoRenderer * render);
    void RemoveRenderer(webrtc::MediaStreamInterface * stream, irender::IVideoRenderer * render);

private:
    talk_base::scoped_refptr<webrtc::PeerConnectionInterface> m_peerConnection;
    boost::shared_ptr<RTCPeerConnection> m_rtcPC;
    std::map<int, boost::shared_ptr<RTCEventHandler> > m_handlers;
    std::vector<talk_base::scoped_refptr<irender::IVideoRenderer> > m_renders;
};

} // namespace iengine


#endif // _WEN_PEER_OBSERVER_H_

