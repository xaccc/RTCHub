#ifndef _WEN_PEER_CONNECTION_H_
#define _WEN_PEER_CONNECTION_H_

#include "WENRtcCommon.h"
#include "WENPeerObserver.h"
#include "WENEventHandler.h"
#include "WENMediaStream.h"

class WebrtcEngineAPI;

namespace iengine {

class RTCPeerConnection : public FB::JSAPIAuto 
{
public:
    RTCPeerConnection();
    virtual ~RTCPeerConnection();

    bool Init(WebrtcEngineAPI * engine,
            std::map<int, boost::shared_ptr<RTCEventHandler> > & handlers, 
            talk_base::scoped_refptr<webrtc::PeerConnectionFactoryInterface> & pcFactory);

    talk_base::scoped_refptr<RTCPeerObserver> getPeer() { return m_peerObserver; }

public:
    // for Webrtc interfaces
    void createOffer(const FB::JSObjectPtr & rtcSdpCallback,
            const FB::JSObjectPtr & rtcErrorCallback, 
            const FB::VariantMap & constraints);

    void createAnswer(const boost::shared_ptr<RTCSessionDescription> & rtcSdp, 
            const FB::JSObjectPtr & rtcSdpCallback, 
            const FB::JSObjectPtr & rtcErrorCallback, 
            const FB::VariantMap & constraints);

    // for session description
    void setLocalDescription(const boost::shared_ptr<RTCSessionDescription> & rtcSdp, 
            const FB::JSObjectPtr & rtcVoidCallback, 
            const FB::JSObjectPtr & rtcErrorCallback);
    boost::shared_ptr<RTCSessionDescription> localDescription();

    void setRemoteDescription(const boost::shared_ptr<RTCSessionDescription> & rtcSdp,
            const FB::JSObjectPtr & rtcVoidCallback, 
            const FB::JSObjectPtr & rtcErrorCallback);
    boost::shared_ptr<RTCSessionDescription> remoteDescription();

    RTCPeerState readyState();

    void updateIce(const std::string & config, const std::string & constraints, bool restart);
    void addIceCandidate(const boost::shared_ptr<RTCIceCandidate> & candidate);
    RTCIceState iceState();

    std::vector<boost::shared_ptr<MediaStream> > & localStreams();
    std::vector<boost::shared_ptr<MediaStream> > & remoteStreams();

    //DataChannel createDataChannel(std::string);
    //RTCEventHandler ondatachannel();

    // for local stream
    void addStream(const FB::variant & stream, const FB::VariantMap & constraints);
    void removeStream(const FB::variant & stream);

    void close ();

    //
    // for render
    // add/remove local/remote render,  0: add, 1: remove
    void setLocalRenderer(const unsigned long render, const int action); 
    void setRemoteRenderer(const unsigned long render, const int action);

    // new interfaces for render
    void addRenderer(const unsigned long stream, const unsigned long render);
    void removeRenderer(const unsigned long stream, const unsigned long render);

    // new ice for offer or answer
    FB_JSAPI_EVENT(onnegotationneeded, 0, ());
    FB_JSAPI_EVENT(onicecandidate, 1, (const FB::variant));
    FB_JSAPI_EVENT(onopen, 0, ());
    FB_JSAPI_EVENT(onstatechange, 1, (int));
    FB_JSAPI_EVENT(onicechange, 1, (int));

    // for remote stream
    FB_JSAPI_EVENT(onaddstream, 1, (const unsigned long));
    FB_JSAPI_EVENT(onremovestream, 1, (const unsigned long));

private:
    // for MediaStream
    std::vector<boost::shared_ptr<MediaStream> > m_localStreams;
    std::vector<boost::shared_ptr<MediaStream> > m_remoteStreams;

    RTCPeerState m_readyState;
    RTCIceState m_iceState;
    talk_base::scoped_refptr<RTCPeerObserver> m_peerObserver;

    // for callback
    std::map<int, boost::shared_ptr<RTCEventHandler> > m_handlers;
    WebrtcEngineAPI * m_engine;
};

}; // namespace iengine

#endif // _WEN_PEER_CONNECTION_H_
