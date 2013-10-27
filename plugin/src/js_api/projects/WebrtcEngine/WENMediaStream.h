#ifndef _WEN_MEDIA_STREAM_H_
#define _WEN_MEDIA_STREAM_H_

#include "JSAPIAuto.h"
#include "WENRtcCommon.h"


/**
 * one MediaStream -> several MediaStreamTrackList 
 * one MediaStreamTrackList -> several MediaStreamTrack
 */

namespace iengine {

class MediaStreamTrack : public FB::JSAPIAuto 
{
public:
    MediaStreamTrack(std::string kind, std::string label);
    virtual ~MediaStreamTrack();

    enum MediaStreamTrackState {
        LIVE = 0,
        MUTED = 1,
        ENDED = 2
    };

    std::string kind() { return m_kind; } // "audio" or "video"
    std::string label() { return m_label; }
    bool enabled() { return m_enabled; }
    void set_enabled(bool enabled) { m_enabled = enabled; }
    unsigned short readyState() { return m_readyState; }
    
    // for event
    //FB_JSAPI_EVENT(onmute, 0, ());
    //FB_JSAPI_EVENT(onunmute, 0, ());
    //FB_JSAPI_EVENT(onended, 0, ());

private:
    std::string m_kind;
    std::string m_label;
    bool m_enabled;
    unsigned short m_readyState;

public:
    bool Init(talk_base::scoped_refptr<webrtc::PeerConnectionFactoryInterface> & pcFactory, 
            std::string aud_in="", std::string aud_out="", std::string vid_uid="");
    talk_base::scoped_refptr<webrtc::MediaStreamTrackInterface> getTrack();
    talk_base::scoped_refptr<webrtc::MediaSourceInterface> getSource();

    // for local video devices
    static FB::VariantMap GetVideoDevices();
    static cricket::VideoCapturer* OpenVideoCaptureDevice(std::string unique_id);

    // for local audio devices
    static FB::VariantList GetAudioDevices(bool bInput);

private:
    talk_base::scoped_refptr<webrtc::MediaStreamTrackInterface> m_track;
    talk_base::scoped_refptr<webrtc::MediaSourceInterface> m_source;
};

class MediaStreamTrackList : public FB::JSAPIAuto 
{
public:
    MediaStreamTrackList();
    virtual ~MediaStreamTrackList();

    unsigned long length();
    boost::shared_ptr<MediaStreamTrack> item(unsigned long index);
    void add(boost::shared_ptr<MediaStreamTrack> track);
    void remove(boost::shared_ptr<MediaStreamTrack> track);

    // event
    //FB_JSAPI_EVENT(onaddtrack, 0, ());
    //FB_JSAPI_EVENT(onremovetrack, 0, ());

private:
    std::vector<boost::shared_ptr<MediaStreamTrack> > m_trackList;
};

class MediaStream : public FB::JSAPIAuto 
{
public:
    MediaStream(const std::string label);
    virtual ~MediaStream();

    std::string label() const { return m_label; }
    boost::shared_ptr<MediaStreamTrackList> audioTracks() { return m_audioTracks; }
    boost::shared_ptr<MediaStreamTrackList> videoTracks() { return m_videoTracks; }
    bool ended() { return m_ended; }
    void set_ended(bool ended) { m_ended = ended; }

    // for extension
    unsigned long getStreamX() { return (unsigned long)m_stream.get(); }

    // event
    //FB_JSAPI_EVENT(onended, 0, ());

private:
    std::string m_label;
    bool m_ended;
    boost::shared_ptr<MediaStreamTrackList> m_audioTracks;
    boost::shared_ptr<MediaStreamTrackList> m_videoTracks;

public:
    void setStream(talk_base::scoped_refptr<webrtc::MediaStreamInterface> stream) { m_stream = stream; }
    talk_base::scoped_refptr<webrtc::MediaStreamInterface> getStream() { return m_stream; }

private:
    talk_base::scoped_refptr<webrtc::MediaStreamInterface> m_stream;
};

} // namespace iengine

#endif // _WEN_MEDIA_STREAM_H_

