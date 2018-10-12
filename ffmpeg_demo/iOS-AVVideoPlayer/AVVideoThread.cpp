//
//  AVVideoThread.cpp
//  ffmpeg_demo
//
//  Created by yuxueqing on 2018/9/20.
//  Copyright © 2018年 yuxueqing. All rights reserved.
//

#include "AVVideoThread.hpp"

namespace AVVideoPlayer {
    
AVVideoThread::AVVideoThread(
AVCodecParameters *params,
AVVideoPLay *call,
const AVDecodePtr& ptr,
const int& maxSize) :
AVDecodeThread(ptr,maxSize)
{
    m_params = params;
    m_videoPlay = call;
    m_synpts = 0;
    m_isPause = false;
}
    
AVVideoThread::~AVVideoThread()
{
    
}
    
bool AVVideoThread::Open(int width, int height)
{
    if (!m_params) return false;
    Clear();
    
    std::lock_guard<std::mutex> lock(m_videoMutex);
    if (m_videoPlay) m_videoPlay->Init(width, height);
    
    bool ret = m_decode->Open(m_params);
    return ret;
}
    
void AVVideoThread::SetPause(bool isPause)
{
    m_isPause = isPause;
}

void AVVideoThread::Start()
{
    m_decodeThread = std::thread([this](){
        pthread_setname_np("com.avvideothread");
        this->StartVideoThread();
    });
}

void AVVideoThread::StartVideoThread()
{
    while (true) {
        AVPacket *pkt = Pop();
        if (!pkt) continue;
        
    }
}
    
}
