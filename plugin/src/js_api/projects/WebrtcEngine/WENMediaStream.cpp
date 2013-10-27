#include "WENMediaStream.h"
#include "WENRtcCommon.h"

namespace iengine {

/**
 * for class MediaStreamTrack
 */

MediaStreamTrack::MediaStreamTrack(std::string kind, std::string label) 
    : m_kind(kind), m_label(label)
{
    m_enabled = false;
    m_readyState = ENDED;
    m_source = NULL;
    m_track = NULL;

    registerProperty("kind", make_property(this, &MediaStreamTrack::kind));
    registerProperty("label", make_property(this, &MediaStreamTrack::label));
    registerProperty("enabled", make_property(this, &MediaStreamTrack::enabled, &MediaStreamTrack::set_enabled));
    registerProperty("readyState", make_property(this, &MediaStreamTrack::readyState));
}

MediaStreamTrack::~MediaStreamTrack() 
{
    LOGD("begin..");
    m_enabled = false;
    m_readyState = ENDED;
    m_source.release();
    m_track.release();
    LOGD("end!");
}

bool MediaStreamTrack::Init(talk_base::scoped_refptr<webrtc::PeerConnectionFactoryInterface> & pcFactory,
        std::string aud_in, std::string aud_out, std::string vid_uid)
{
    LOGD("begin..(MediaStreamTrack)");
    if (!pcFactory) {
        LOGE("invalid PeerConnectionFactory");
        return false;
    }

    if (m_kind == kAudioKind) {
        if (!m_source) {
            // For the latest webrtc version
            m_source = pcFactory->CreateAudioSource(NULL);
        }
        // m_source is NULL or not NULL, all OK
        LOGD("create audio track: "<<m_label);
        m_track = pcFactory->CreateAudioTrack(m_label, (webrtc::AudioSourceInterface *)(m_source.get()));
    }else if (m_kind == kVideoKind) {
        if (!m_source) {
            cricket::VideoCapturer* capturer = OpenVideoCaptureDevice(vid_uid);
            if (capturer) {
                LOGI("capturer="<<capturer);
                m_source = pcFactory->CreateVideoSource(capturer, NULL);
            }
        }
        if (m_source) {
            LOGD("create video track: "<<m_label<<", m_source="<<m_source);
            m_track = pcFactory->CreateVideoTrack(m_label, (webrtc::VideoSourceInterface *)(m_source.get()));
        }else {
            LOGE("no invalid video captuer to CreateVideoTrack");
        }
    }

    LOGI("m_kind="<<m_kind<<", m_track="<<m_track<<", m_source="<<m_source);
    return (m_track != NULL);
}

talk_base::scoped_refptr<webrtc::MediaStreamTrackInterface> MediaStreamTrack::getTrack() 
{
    return m_track;
}

talk_base::scoped_refptr<webrtc::MediaSourceInterface> MediaStreamTrack::getSource() 
{
    return m_source;
}

cricket::VideoCapturer* MediaStreamTrack::OpenVideoCaptureDevice(std::string unique_id) 
{
    talk_base::scoped_ptr<cricket::DeviceManagerInterface> dev_manager(
            cricket::DeviceManagerFactory::Create());
    if (!dev_manager->Init()) {
        LOGE("fail to create DeviceManager");
        return NULL;
    }

    std::vector<cricket::Device> devs;
    if (!dev_manager->GetVideoCaptureDevices(&devs)) {
        LOGE("fail to get Video Capture Devices");
        return NULL;
    }

    cricket::VideoCapturer* capturer = NULL;
    std::vector<cricket::Device>::iterator dev_it = devs.begin();
    for (; dev_it != devs.end(); ++dev_it) {
        std::string key = (*dev_it).id;
        if (!unique_id.empty() && unique_id != key) {
            continue;
        }
        capturer = dev_manager->CreateVideoCapturer(*dev_it);
        if (capturer != NULL)
            break;
    }

    // TODO: choose the best format

    return capturer;
}

FB::VariantMap MediaStreamTrack::GetVideoDevices()
{
    FB::VariantMap devices;
    std::string key;
    std::string val;

    talk_base::scoped_ptr<cricket::DeviceManagerInterface> dev_manager(
            cricket::DeviceManagerFactory::Create());
    if (!dev_manager->Init()) {
        LOGE("fail to create DeviceManager");
        return devices;
    }

    std::vector<cricket::Device> devs;
    if (!dev_manager->GetVideoCaptureDevices(&devs)) {
        LOGE("fail to get Video Capture Devices");
        return devices;
    }

    std::vector<cricket::Device>::iterator dev_it = devs.begin();
    for (; dev_it != devs.end(); ++dev_it) {
        key = (*dev_it).id;
        val = (*dev_it).name;
        devices[key] = val;

        std::string msg("Capture device [id = ");
        msg += key;
        msg += ", name = ";
        msg += (val + "]");
        LOGI(msg);
    }

    return devices;
}

FB::VariantList MediaStreamTrack::GetAudioDevices(bool bInput)
{
    FB::VariantList devices;
    std::vector<cricket::Device> devicelist;

    static bool initTried = false;
    static talk_base::scoped_ptr<cricket::DeviceManagerInterface> devmgr(cricket::DeviceManagerFactory::Create());

    if(false == initTried)
    {
        if(false == devmgr->Init())
        {
            LOGE("Can't init device manager");
            return devices;
        }
        initTried = true;
    }

    if(true == bInput)
    {
        devmgr->GetAudioInputDevices(&devicelist);
    }
    else
    {
        devmgr->GetAudioOutputDevices(&devicelist);
    }

    for(size_t i=0; i<devicelist.size(); i++)
    {
        devices.push_back(FB::variant(devicelist[i].name));
    }

    return devices;
}


/**
 * for class MediaStreamTrackList
 */

MediaStreamTrackList::MediaStreamTrackList()
{
    registerProperty("length", make_property(this, &MediaStreamTrackList::length));
    registerMethod("item", make_method(this, &MediaStreamTrackList::item));
    registerMethod("add", make_method(this, &MediaStreamTrackList::add));
    registerMethod("remove", make_method(this, &MediaStreamTrackList::remove));
}

MediaStreamTrackList::~MediaStreamTrackList()
{
    LOGD("begin..");
    m_trackList.clear();
    LOGD("end!");
}

unsigned long MediaStreamTrackList::length() 
{ 
    return m_trackList.size(); 
}

boost::shared_ptr<MediaStreamTrack> MediaStreamTrackList::item(unsigned long index) 
{
    if (index >= m_trackList.size()) {
        throw FB::script_error("The index is invalid");
    }
    return m_trackList[index];
}

void MediaStreamTrackList::add(boost::shared_ptr<MediaStreamTrack> track) 
{
    m_trackList.push_back(track);
}

void MediaStreamTrackList::remove(boost::shared_ptr<MediaStreamTrack> track) 
{
    std::vector<boost::shared_ptr<MediaStreamTrack> >::iterator iter = m_trackList.begin();
    for (; iter != m_trackList.end(); iter++) {
        if ((*iter) == track) {
            iter = m_trackList.erase(iter);
            break;
        }
    }
}


/**
 * for class MediaStream
 */

MediaStream::MediaStream(std::string label) : m_label(label)
{
    m_ended = false;
    m_stream = NULL;
    m_audioTracks = boost::make_shared<MediaStreamTrackList>();
    m_videoTracks = boost::make_shared<MediaStreamTrackList>();

    registerProperty("label", make_property(this, &MediaStream::label));
    registerProperty("audioTracks", make_property(this, &MediaStream::audioTracks));
    registerProperty("videoTracks", make_property(this, &MediaStream::videoTracks));
    registerProperty("ended", make_property(this, &MediaStream::ended, &MediaStream::set_ended));

    registerMethod("getStreamX", make_method(this, &MediaStream::getStreamX));
}

MediaStream::~MediaStream()
{
    LOGD("begin..");
    m_ended = true;
    m_stream.release();
    m_audioTracks.reset();
    m_videoTracks.reset();
    LOGD("end!");
}

} // namespace iengine
