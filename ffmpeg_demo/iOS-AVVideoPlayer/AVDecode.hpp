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
    
    virtual bool Open(AVCodecParameters *parameters);
    
    virtual bool Send(AVPacket *packet);
    
    virtual AVModeFrame* RecvFrame();
    
    AVCodecContext *codecContext() const;
    
    virtual void Close();
    
    virtual void Clear();
    
public:
    AVDecode();
    virtual ~AVDecode();
    
public:
    std::atomic<bool> m_isAudio; ;
    
    long long m_pts = 0;
    
protected:
    AVCodecContext *m_codecContext { nullptr };
    std::mutex m_mutex;
};
    
}


#endif /* AVDecode_hpp */
