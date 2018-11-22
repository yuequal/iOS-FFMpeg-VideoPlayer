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

template <typename T,std::function<void(T **)> Free>
struct AVObjectFree  {
    void operator()(T *t) const {  Free(&t); };
};
struct AVCodecContextFree {
  void operator()(AVCodecContext* ctx) const { avcodec_free_context(&ctx); }
};
struct AVFrameFree {
    void operator()(AVFrame *frame) const { av_frame_free(&frame); }
};
struct AVModeFrameFree {
    void operator()(AVModeFrame *modeFrame) const { av_free_mode_frame(&modeFrame);  }
};

extern void AVFreePacket(AVPacket **packet);

namespace AVVideoPlayer{
    
class AVDecode
{
public:
    
    virtual bool Open(AVCodecParameters *parameters);
    
    virtual bool Send(AVPacket *packet);
    
    virtual int32_t AVRecvFrame(AVCodecContext *context, AVModeFrame *av_frame, int flags);
    
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
    
private:
    static void AVFreeBuffer(void *context, uint8_t *data);
    
protected:
    AVCodecContext *m_codecContext { nullptr };
    std::mutex m_mutex;
};
    
}


#endif /* AVDecode_hpp */
