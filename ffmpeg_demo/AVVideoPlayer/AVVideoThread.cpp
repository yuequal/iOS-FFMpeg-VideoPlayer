//
//  AVVideoThread.cpp
//  ffmpeg_demo
//
//  Created by yuxueqing on 2018/9/20.
//  Copyright © 2018年 yuxueqing. All rights reserved.
//

#include "AVVideoThread.hpp"

namespace AVVideoPlayer {
    
    AVVideoThread::AVVideoThread(AVCodecParameters *params, AVVideoCall *call, const AVDecodePtr& ptr, const int& maxSize) :
    AVDecodeThread(ptr,maxSize)
{
    m_params = params;
    m_call = call;
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
    if (m_call) m_call->Init(width, height);
    
    bool ret = m_decode->Open(m_params);
    return ret;
}
    
void AVVideoThread::SetPause(bool isPause)
{
    m_isPause = isPause;
}
    

void AVVideoThread::StartVideoThread()
{
    while (!m_isExit.load()) {
        std::unique_lock<std::mutex> lock(m_videoMutex);
        m_decodeThreadCond.wait(lock, [&]{
            return m_isPause.load();
        });
        m_decodeThreadCond.wait(lock, [&]{
            return m_synpts >0 && m_synpts < m_decode->pts;
        });
        
        AVPacket *packet = Pop();
        bool ret = m_decode->Send(packet);
        if (!ret) {
            lock.unlock();
            continue;
        }
        
        while (!m_isExit) {
            AVModeFrame *mFrame = m_decode->RecvFrame();
            if (!mFrame) break;
            if (m_call) m_call->Repaint(mFrame->frame());
        }
    }
}
    
}
