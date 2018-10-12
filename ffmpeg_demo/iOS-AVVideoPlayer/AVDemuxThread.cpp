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
{

}

AVDemuxThread::~AVDemuxThread()
{
    
}

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
    m_state = AVState::AVPause;
}

void AVDemuxThread::StartDemux(const char *url, const AVDemuxCallBack& cb, void *context)
{
    m_fileUrl = url;
}

bool AVDemuxThread::Open(const char *url, AVVideoPLay *videoCall)
{
    m_videoPlay = videoCall;
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
//    std::lock_guard<std::mutex> lock(m_mutex);
//    if (!m_demux) m_demux = std::make_shared<AVDemux>();
//    bool ret = m_demux->Open(m_fileUrl);
//    if (!ret) return;
//    AVVideoThread *videoThread = new AVVideoThread(m_demux->CopyVideoParams(),nullptr,nullptr,10);
//
//    if (!m_videoThread) m_videoThread = std::make_shared<AVVideoThread>(m_demux->CopyVideoParams(),m_videoPlay,std::make_shared<AVDecode>(),10);
//
//    if (!m_videoThread->Open(100, 100)) {
//        return;
//    }
//    while (true) {
//        AVPacket *packet = m_demux->Read();
//        if (!packet) {
//            continue;
//        }
//        if (!m_demux->IsAudio(packet)) {
//            m_videoThread->Push(packet);
//        }
//    }
//    m_videoThread->StartVideoThread();
//    m_totalMills = m_demux->totalMills;
}
    
}
