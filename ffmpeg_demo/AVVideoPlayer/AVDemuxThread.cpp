//
//  AVDemuxThread.cpp
//  ffmpeg_demo
//
//  Created by yuxueqing on 2018/9/11.
//  Copyright © 2018年 yuxueqing. All rights reserved.
//

#include "AVDemuxThread.hpp"

namespace AVVideoPlayer {
    
AVDemuxThread::AVDemuxThread()
{}

AVDemuxThread::~AVDemuxThread()
{}

void AVDemuxThread::Clear()
{
    m_mutex.lock();
    if (m_demux) m_demux->Clear();
    m_mutex.unlock();
}

void AVDemuxThread::Seek(double pos)
{
    Clear();
    m_mutex.lock();
    if (m_demux)
        m_demux->Seek(pos);
    m_mutex.unlock();
}

void AVDemuxThread::SetPause(bool isPause)
{
    this->m_isPause = isPause;
}

void AVDemuxThread::StartDemux(const char *url, const AVDemuxCallBack& cb, void *context)
{}

bool AVDemuxThread::Open(const char *url, AVVideoPLay *videoCall)
{
    return false;
}

void AVDemuxThread::Start()
{
    m_demuxThread = std::thread([this](){
        pthread_setname_np("com.avideoplayer.AVDemuxThread");
        this->DemuxThread();
    });
}
    
void AVDemuxThread::DemuxThread()
{
    
}
}
