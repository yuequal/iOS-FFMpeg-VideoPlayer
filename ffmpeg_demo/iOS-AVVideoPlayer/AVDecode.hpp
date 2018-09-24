//
//  AVDecode.hpp
//  ffmpeg_demo
//
//  Created by yuxueqing on 2018/9/9.
//  Copyright © 2018年 yuxueqing. All rights reserved.
//

#ifndef AVDecode_hpp
#define AVDecode_hpp

#include <mutex>
#include "AVModeFrame.hpp"
extern "C"{
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavformat/avformat.h>
}


extern void AVFreePacket(AVPacket **packet);

namespace AVVideoPlayer{
    
class AVDecode
{
public:
    bool isAudio = false;
    
    long long pts = 0;
    
    virtual bool Open(AVCodecParameters *parameters);
    
    virtual bool Send(AVPacket *packet);
    
    virtual AVModeFrame* RecvFrame();
    
    virtual void Close();
    
    virtual void Clear();
    
public:
    AVDecode();
    virtual ~AVDecode();
    
protected:
    AVCodecContext *m_codecContext = 0;
    std::mutex m_mutex;
};
    
}


#endif /* AVDecode_hpp */
