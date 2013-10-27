#ifndef _WEN_RTC_SDP_H_
#define _WEN_RTC_SDP_H_

#include "JSAPIAuto.h"
#include "WENRtcCommon.h"

namespace iengine {

class RTCSessionDescription : public FB::JSAPIAuto 
{
public:
    RTCSessionDescription();
    RTCSessionDescription(const std::string &type, const std::string &sdp);
    virtual ~RTCSessionDescription();

    std::string & type() { return m_type; }
    void set_type(const std::string & type) { m_type = type; }
    std::string & sdp() { return m_sdp; }
    void set_sdp(const std::string & sdp) { m_sdp = sdp; }

private:
    void Init_();

    std::string m_type;
    std::string m_sdp;
};

class RTCIceCandidate : public FB::JSAPIAuto 
{
public:
    RTCIceCandidate();
    RTCIceCandidate(const std::string & candidate, const std::string & sdpMid, unsigned short sdpMLineIndex);
    virtual ~RTCIceCandidate();

    std::string & candidate() { return m_candidate; }
    void set_candidate(const std::string & candidate) { m_candidate = candidate; }
    std::string & sdpMid() { return m_sdpMid; }
    void set_sdpMid(const std::string & sdpMid) { m_sdpMid = sdpMid; }
    unsigned short sdpMLineIndex() { return m_sdpMLineIndex; }
    void set_sdpMLineIndex(unsigned short sdpMLineIndex) { m_sdpMLineIndex = sdpMLineIndex; }

private:
    void Init_();

    std::string m_candidate; // this is sdp in actual
    std::string m_sdpMid;
    unsigned short m_sdpMLineIndex;
};

} // namespace iengine

#endif // _WENMEDIASDP_H_ 
