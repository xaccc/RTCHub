#ifndef _WEN_RTC_COMMON_H_
#define _WEN_RTC_COMMON_H_

#include <time.h>

#include <string>
#include <map>
#include <queue>
#include <vector>
#include <iostream>
#include <boost/weak_ptr.hpp>
#include <boost/thread/mutex.hpp>

#include "talk/base/common.h"
#include "talk/base/json.h"
#include "talk/base/scoped_ptr.h"
#include "talk/base/physicalsocketserver.h"
#include "talk/media/base/mediaengine.h"
#include "talk/app/webrtc/mediastreaminterface.h"
#include "talk/app/webrtc/videosourceinterface.h"
#include "talk/app/webrtc/peerconnection.h"
#include "talk/app/webrtc/peerconnectionfactory.h"

namespace iengine {

#ifdef WIN32
#define PID 1001
#define TID 1001001
#else
#define PID ::getpid()
#define TID ::pthread_self()
#endif

// Log Trace
const int s_logid_= (unsigned int) time(NULL);

#ifndef LOG_LOCAL
#define LOG_LOCAL

#undef LOG_
#undef LOGD
#undef LOGI
#undef LOGW
#undef LOGE

#undef LOG__
#undef LOGD_
#undef LOGI_
#undef LOGW_
#undef LOGE_

#define LOG_(out, l, p)    {                        \
    (out)                                           \
    << "[" << l << "]"                              \
    << "[" << PID << "]"                            \
    << "[" << iengine::s_logid_ << "]"              \
    << "[" << __FILE__ << ":"                       \
    << " " << __LINE__ << "]"                       \
    << " " << __FUNCTION__ << ","                   \
    << " " << p << std::endl;}
#define LOG__(out, l, p)    {                       \
    (out)                                           \
    << "[" << l << "]"                              \
    << "[" << PID << "]"                            \
    << "[" << iengine::s_logid_ << "]"              \
    << "[" << __FILE__ << ":"                       \
    << " " << __LINE__ << "]"                       \
    << " " << __FUNCTION__ << ","                   \
    << " <" << #p << "> is false or NULL."<< std::endl;}

// for common log
#define LOGD(p) LOG_(std::cout, "DEBUG", p)
#define LOGI(p) LOG_(std::cout, "INFO",  p)
#define LOGW(p) LOG_(std::cout, "WARN",  p)
#define LOGE(p) LOG_(std::cerr, "ERROR", p)

// for detail log
#define LOGD_(p) LOG__(std::cout, "DEBUG", p)
#define LOGI_(p) LOG__(std::cout, "INFO",  p)
#define LOGW_(p) LOG__(std::cout, "WARN",  p)
#define LOGE_(p) LOG__(std::cerr, "ERROR", p)

// for condition check
#ifndef return_if_fail
#define return_if_fail(p)       { if (!(p)) { LOGE_(p); return; } }
#define returnv_if_fail(p, v)   { if (!(p)) { LOGE_(p); return (v); } }
#define return_assert(p)        { if (!(p)) { LOGW_(p); return; } }
#define returnv_assert(p, v)    { if (!(p)) { LOGW_(p); return (v); } }
#define break_assert(p)         { if (!(p)) { LOGW_(p); break; } }
#define continue_assert(p, v)   { if (!(p)) { LOGW_(p); continue; } }
#endif

#endif // LOG_LOCAL


// for sleep
#ifdef WIN32
#define sleep_ms(n)     Sleep(n)
#else
#define sleep_ms(n)     usleep((n)*1000)
#endif

// for render
const int kDefaultRenderWidth   = 352;
const int kDefaultRenderHeight  = 288;


// Names for media kind
const char kAudioKind[] = "audio";
const char kVideoKind[] = "video";

// Names for stream/video/audio label
const char kAudioLabel[] = "audio_label";
const char kVideoLabel[] = "video_label";
const char kStreamLabel[] = "stream_label";

// Names used for a IceCandidate JSON object.
const char kCandidateSdpMidName[] = "sdpMid";
const char kCandidateSdpMlineIndexName[] = "sdpMLineIndex";
const char kCandidateSdpName[] = "candidate";

// Names used for a SessionDescription JSON object.
const char kSessionDescriptionTypeName[] = "type";
const char kSessionDescriptionSdpName[] = "sdp";

// default ice server
const char kDefaultIceServer[] = "stun:stun.l.google.com:19302";


// max size of key string
const int kMaxKeySize = 32;

//
// RTC SDP Type
enum RTCSdpType {
    kSdpOffer,
    kSdpPranswer,
    kSdpAnswer,
    kSdpAll
};

const char kRTCSdpType[][kMaxKeySize] = {
    "offer",
    "pranswer",
    "answer"
};

//
// RTC Peer State
enum RTCPeerState {
    kPeerNew,
    kPeerOpening,
    kPeerActive,
    kPeerClosing,
    kPeerClosed,
    kPeerStateAll
};

const char kRTCPeerState[][kMaxKeySize] = {
    "new",
    "opening",
    "active",
    "closing",
    "closed"
};

//
// RTC Ice State
enum RTCIceState {
    kIceNew,
    kIceGathering,
    kIceWaiting,
    kIceChecking,
    kIceConnected,
    kIceCompleted,
    kIceFailed,
    kIceClosed,
    kIceStateAll
};

const char kRTCIceState[][kMaxKeySize] = {
    "new",
    "gathering",
    "waiting",
    "checking",
    "connected",
    "completed",
    "failed",
    "closed"
};

} // namespace iengine


#endif // _WEN_RTC_COMMON_H_

