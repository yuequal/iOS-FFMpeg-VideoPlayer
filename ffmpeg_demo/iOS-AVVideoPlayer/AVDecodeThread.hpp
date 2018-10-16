//
//  AVDecodeThread.hpp
//  ffmpeg_demo
//
//  Created by yuxueqing on 2018/9/19.
//  Copyright © 2018年 yuxueqing. All rights reserved.
//

#ifndef AVDecodeThread_hpp
#define AVDecodeThread_hpp

#include <list>
#include <mutex>
#include <thread>
extern "C"{
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavformat/avformat.h>
}
#include "AVDecode.hpp"
#include "AVPacketQueue.hpp"
namespace AVVideoPlayer {
    
typedef std::shared_ptr<AVDecode> AVDecodePtr;

class AVDecodeThread
{
public:
    AVDecodeThread(const AVDecodePtr& decodePtr,const int& maxSize);
    virtual ~AVDecodeThread();

public:
    virtual int32_t Push(AVPacket *packet);
    
    virtual void Clear();
    
    virtual void Close();
    
    virtual bool Empty() ;
    
    virtual AVPacket *Pop();
    
    virtual int32_t Size();
    
public:
    int maxLimit = 10;
    std::atomic<bool> m_isExit;
    
protected:
    std::list<AVPacket *> m_packets;
    std::mutex m_mutex;
    std::thread m_decodeThread;
    std::shared_ptr<AVDecode> m_decode;
    std::condition_variable m_decodeThreadCond;
    AVPacketQueue<AVPacket> m_packetQueue;
};
}

#endif /* AVDecodeThread_hpp */
