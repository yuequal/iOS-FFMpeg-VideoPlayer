//
//  AVDemux.hpp
//  ffmpeg_demo
//
//  Created by yuxueqing on 2018/9/7.
//  Copyright © 2018年 yuxueqing. All rights reserved.
//

#ifndef AVDemux_hpp
#define AVDemux_hpp


#include <mutex>
#include <thread>
extern "C"{
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavformat/avformat.h>
}

namespace AVVideoPlayer {
    
class AVDemux {
    
public:
    AVDemux(AVFormatContext *formatContext);
    virtual ~AVDemux();
    
public:
    
    virtual bool Open(const char *url);
    
    void Start();
    
    virtual void ReadPacket(AVPacket *packet);
    
    virtual AVPacket *Read();
    
    virtual bool IsAudio(AVPacket *packet);
    
    virtual AVCodecParameters *CopyVideoParams();
    
    virtual AVCodecParameters *CopyAudioParams();
    
    virtual bool Seek(double pos);
    
    virtual void Clear();
    
    virtual void Close();
    
    int VStreamIndex() const;
    
    int AStreamIndex() const;
    
    AVFormatContext *FormatContext() const;
    
public:
    
    long long totalMills = 0;
    
    int width = 0;
    int height = 0;
    int sampleRate = 0;
    int channels = 0;
private:
    
    char error[1024] = { 0 };
    std::mutex m_mutex;
    
    int videoStream = 0;
    int audioStream = 0;
    
    AVFormatContext *m_formatContex = 0;
    std::thread m_demuxThread;
};
}

#endif /* AVDemux_hpp */
