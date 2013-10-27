#include "WENEventHandler.h"

#include "WebrtcEngineAPI.h"
#include "WENPeerConnection.h"
#include "WENMediaStream.h"
#include "WENRtcSdp.h"
#include "WENIWebrtcRenderer.h"


namespace iengine {

RTCEventHandler::RTCEventHandler(HandlerLabel_t label)
{
    m_label = label;
    m_quit = false;
}

RTCEventHandler::~RTCEventHandler()
{
}

void RTCEventHandler::process(HandlerKey_t key, boost::shared_ptr<WebrtcEngineAPI> ptr, FB::VariantList args) 
{
    FB::variant obj(ptr);
    process(key, obj, args);
}

void RTCEventHandler::process(HandlerKey_t key, boost::shared_ptr<RTCPeerConnection> ptr, FB::VariantList args) 
{
    FB::variant obj(ptr);
    process(key, obj, args);
}

void RTCEventHandler::registerJSCallback(HandlerKey_t key, FB::JSObjectPtr ptr) 
{
    m_jscallbacks[key] = ptr;
}

void RTCEventHandler::processJSCallback(HandlerKey_t key, FB::VariantList args, bool async) 
{
    jsCallbackInvoke(key, args, async);
}

int RTCEventHandler::workerBee(void)
{
    do {
        sleep_ms(100);

        int key = kKeyUnknown;
        body_t body;

        m_mutex.lock();
        if (!m_events.empty()) {
            LOGD("queue size = "<<m_events.size());
            event_t event = m_events.front();
            key = event.first;
            body = event.second;
            m_events.pop();
        }
        m_mutex.unlock();

        if (key <= kKeyUnknown || key >= kKeyAll) {
            m_quit = (key == kKeyQuit);
            if (m_quit) {
                LOGD("m_quit is true and then exit");
            }
            continue;
        }

        LOGD("label="<<m_label<<", key="<<key);
        switch (m_label) 
        { 
        case kActionLabel:
            processAction((HandlerKey_t)key, body);
            break;
        case kEventLabel:
            processEvent((HandlerKey_t)key, body);
            break;
        case kJSCallbackLabel:
            break;
        default:
            break;
        }
    }while(!m_quit);

    return 0;
}

void RTCEventHandler::processAction(HandlerKey_t key, body_t & body)
{
    boost::shared_ptr<RTCPeerConnection> ptr = 
        body.first.convert_cast<boost::shared_ptr<RTCPeerConnection> >();
    FB::VariantList argv = body.second;

    switch (key) {
    case kAddStream:
        if (argv.size() == 2) {
            boost::shared_ptr<MediaStream> arg1 = 
                argv[0].convert_cast<boost::shared_ptr<MediaStream> >();
            ptr->getPeer()->AddStream(arg1->getStream(), NULL);
        }
        break;
    case kRemoveStream:
        if (argv.size() == 1) {
            boost::shared_ptr<MediaStream> arg1 = 
                argv[0].convert_cast<boost::shared_ptr<MediaStream> >();
            ptr->getPeer()->RemoveStream(arg1->getStream());
        }
        break;
    case kCreateOffer:
        if (argv.size() == 1) {
            ptr->getPeer()->CreateOffer();
        }
        break;
    case kCreateAnswer:
        if (argv.size() == 2) {
            boost::shared_ptr<RTCSessionDescription> arg1 = 
                argv[0].convert_cast<boost::shared_ptr<RTCSessionDescription> >();
            ptr->getPeer()->CreateAnswer(arg1);
        }
        break;
    case kSetLocalDescription:
    case kSetRemoteDescription:
        if (argv.size() == 1) {
            boost::shared_ptr<RTCSessionDescription> arg1 = 
                argv[0].convert_cast<boost::shared_ptr<RTCSessionDescription> >();
            if (key == kSetLocalDescription)
                ptr->getPeer()->SetLocalDescription(arg1);
            else
                ptr->getPeer()->SetRemoteDescription(arg1);
        }
        break;
    case kAddIceCandidate:
        if (argv.size() == 1) {
            boost::shared_ptr<RTCIceCandidate> arg1 = 
                argv[0].convert_cast<boost::shared_ptr<RTCIceCandidate> >();
            ptr->getPeer()->AddIceCandidate(arg1);
        }
        break;
    case kSetLocalRenderer:
    case kSetRemoteRenderer:
        if (argv.size() == 2) {
            unsigned long render = 0;
            int action = 0;

            try {
                render = argv[0].convert_cast<unsigned long>();
                action = argv[1].convert_cast<int>();
            } catch (const FB::bad_variant_cast& ex) {
                LOGE("convert_cast err, from "<<ex.from <<" to "<<ex.to);
                return;
            }

            irender::IVideoRenderer * prender = (irender::IVideoRenderer *)render;
            LOGD("prender = "<<prender<<", render="<<render);
            if (!prender) {
                LOGE("invalid renderer");
                return;
            }

            if (key == kSetLocalRenderer)
                ptr->getPeer()->SetLocalRenderer(prender, action);
            else
                ptr->getPeer()->SetRemoteRenderer(prender, action);
        }
        break;
    case kAddRenderer:
    case kRemoveRenderer:
        if (argv.size() == 2) {
            unsigned long stream = 0;
            unsigned long render = 0;

            LOGD("kAddRenderer/kRemoveRenderer begin");
            try {
                stream = argv[0].convert_cast<unsigned long>();
                render = argv[1].convert_cast<unsigned long>();
            } catch (const FB::bad_variant_cast& ex) {
                LOGE("convert_cast err, from "<<ex.from <<" to "<<ex.to);
                return;
            }

            LOGD("stream="<<stream<<", render="<<render);
            webrtc::MediaStreamInterface *pstream = (webrtc::MediaStreamInterface *)stream;
            irender::IVideoRenderer *prender = (irender::IVideoRenderer *)render;
            if (key == kAddRenderer)
                ptr->getPeer()->AddRenderer(pstream, prender);
            else
                ptr->getPeer()->RemoveRenderer(pstream, prender);
        }
        break;
    default:
        LOGW("invalid action key = "<<key);
        break;
    }
}

void RTCEventHandler::processEvent(HandlerKey_t key, body_t & body)
{
    FB::VariantList argv = body.second;
    boost::shared_ptr<RTCPeerConnection> ptr = 
        body.first.convert_cast<boost::shared_ptr<RTCPeerConnection> >();

    switch (key) {
    case konnegotationneeded:
        ptr->fire_onnegotationneeded();
        break;
    case konicecandidate:
        if (argv.size() == 1) {
            ptr->fire_onicecandidate(argv[0].convert_cast<boost::shared_ptr<RTCIceCandidate> >());
        }
        break;
    case konopen:
        ptr->fire_onopen();
        break;
    case konstatechange:
        if (argv.size() == 1) {
            ptr->fire_onstatechange(argv[0].convert_cast<int>());
        }
        break;
    case konicechange:
        if (argv.size() == 1) {
            ptr->fire_onicechange(argv[0].convert_cast<int>());
        }
        break;
    case konaddstream:
        if (argv.size() == 1) {
            ptr->fire_onaddstream(argv[0].convert_cast<unsigned long>());
        }
        break;
    case konremovestream:
        if (argv.size() == 1) {
            ptr->fire_onremovestream(argv[0].convert_cast<unsigned long>());
        }
        break;
    default:
        LOGW("invalid event key = "<<key);
        break;
    }
}

void RTCEventHandler::jsCallbackInvoke(HandlerKey_t key, FB::VariantList & args, bool async)
{
    if (m_jscallbacks.find(key) != m_jscallbacks.end()) {
        if (m_jscallbacks[key].get())
            if (async)
                m_jscallbacks[key]->InvokeAsync("", args);
            else
                m_jscallbacks[key]->Invoke("", args);
    }
}

} // namespace iengine

