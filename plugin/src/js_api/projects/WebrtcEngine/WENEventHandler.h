#ifndef _WEN_EVENT_HANDLER_H_
#define _WEN_EVENT_HANDLER_H_

#include "JSAPIAuto.h"
#include "ThreadSingle.h"
#include "WENRtcCommon.h"


class WebrtcEngineAPI;

namespace iengine {

enum HandlerLabel_t {
    kActionLabel,
    kEventLabel,
    kJSCallbackLabel,
};

enum HandlerKey_t {
    kKeyQuit = -1,
    kKeyUnknown = 0,

    // for kActionLabel
    kAddStream  = 10,
    kRemoveStream,
    kCreateOffer,
    kCreateAnswer,
    kSetLocalDescription,
    kSetRemoteDescription,
    kAddIceCandidate,
    kSetLocalRenderer,
    kSetRemoteRenderer,
    kAddRenderer,
    kRemoveRenderer,

    // for kEventLabel
    konnegotationneeded = 50,
    konicecandidate,
    konopen,
    konstatechange,
    konicechange,
    konaddstream,
    konremovestream,

    // for kJSCallbackLabel
    kgetUserMediaOk = 100,
    kgetUserMediaError,
    kcreateOfferAnswerOk,
    kcreateOfferAnswerError,
    ksetLocalSessionDescriptionVoid,
    ksetLocalSessionDescriptionError,
    ksetRemoteSessionDescriptionVoid,
    ksetRemoteSessionDescriptionError,

    kKeyAll
};


class RTCPeerConnection;

class RTCEventHandler : public ThreadSingle 
{
typedef std::pair<FB::variant, FB::VariantList> body_t;
typedef std::pair<int, body_t> event_t;

public:
    RTCEventHandler(HandlerLabel_t label);
    virtual ~RTCEventHandler();

    void process(HandlerKey_t key, boost::shared_ptr<WebrtcEngineAPI> ptr, FB::VariantList args);
    void process(HandlerKey_t key, boost::shared_ptr<RTCPeerConnection> ptr, FB::VariantList args);

    void registerJSCallback(HandlerKey_t key, FB::JSObjectPtr ptr);
    void processJSCallback(HandlerKey_t key, FB::VariantList args, bool async=false);

protected:
    void process(HandlerKey_t key, FB::variant obj, FB::VariantList & args) {
        m_mutex.lock();
        m_events.push(event_t(key, body_t(obj, args)));
        m_mutex.unlock();
    }

    void processAction(HandlerKey_t key, body_t & body);
    void processEvent(HandlerKey_t key, body_t & body);
    void jsCallbackInvoke(HandlerKey_t key, FB::VariantList & args, bool async);
    
protected:
    int workerBee(void);

private:
    bool m_quit;

    HandlerLabel_t m_label;
    std::queue<event_t> m_events;
    std::map<int, FB::JSObjectPtr> m_jscallbacks;

    boost::mutex m_mutex;
};

} // namespace iengine


#endif // _WEN_EVENT_HANDLER_H_
