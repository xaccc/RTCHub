#include "WENRtcSdp.h"

namespace iengine {

/**
 * for class RTCSessionDescription
 */

RTCSessionDescription::RTCSessionDescription()
{
    Init_();
}

RTCSessionDescription::RTCSessionDescription(const std::string &type, const std::string &sdp) 
    : m_type(type), m_sdp(sdp)
{
    Init_();
}

void RTCSessionDescription::Init_()
{
    registerProperty("type", make_property(this,
                &RTCSessionDescription::type, &RTCSessionDescription::set_type));
    registerProperty("sdp", make_property(this,
                &RTCSessionDescription::sdp, &RTCSessionDescription::set_sdp));
}

RTCSessionDescription::~RTCSessionDescription()
{
    LOGD("begin..");
}


/**
 * for class RTCIceCandidate
 */

RTCIceCandidate::RTCIceCandidate()
{
    Init_();
}

RTCIceCandidate::RTCIceCandidate(const std::string & candidate, 
        const std::string & sdpMid, unsigned short sdpMLineIndex)
    : m_candidate(candidate), m_sdpMid(sdpMid), m_sdpMLineIndex(sdpMLineIndex)
{
    Init_();
}

void RTCIceCandidate::Init_()
{
    registerProperty("candidate", make_property(this,
                &RTCIceCandidate::candidate, &RTCIceCandidate::set_candidate));
    registerProperty("sdpMid", make_property(this,
                &RTCIceCandidate::sdpMid, &RTCIceCandidate::set_sdpMid));
    registerProperty("sdpMLineIndex", make_property(this,
                &RTCIceCandidate::sdpMLineIndex, &RTCIceCandidate::set_sdpMLineIndex));
}

RTCIceCandidate::~RTCIceCandidate()
{
    LOGD("begin..");
}

} // namespace iengine
