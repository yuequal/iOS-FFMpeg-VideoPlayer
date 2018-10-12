//
//  AVDecodeThread.cpp
//  ffmpeg_demo
//
//  Created by yuxueqing on 2018/9/19.
//  Copyright © 2018年 yuxueqing. All rights reserved.
//

#include "AVDecodeThread.hpp"

namespace AVVideoPlayer {
    
AVDecodeThread::AVDecodeThread(const AVDecodePtr& decodePtr, const int& maxSize)
{
    this->m_decode = decodePtr;
    this->maxLimit = maxSize;
}

AVDecodeThread::~AVDecodeThread()
{
    this->m_isExit = false;
}
    
void AVDecodeThread::Close()
{
    Clear();
    if (m_isExit.load()) {
        m_isExit = true;
    }

    m_decode->Close();
    
    std::lock_guard<std::mutex> lock(m_mutex);
    m_decode.reset();
}

void AVDecodeThread::Clear()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    m_decode->Clear();
    while (!m_packets.empty()) {
        AVPacket *packet = m_packets.front();
        av_packet_unref(packet);
        AVFreePacket(&packet);
        m_packets.pop_front();
    }
}
void AVDecodeThread::Push(AVPacket* packet)
{
    if (!packet || !packet->data || packet->size <= 0) return;
    while (!m_isExit.load()) {
        std::unique_lock<std::mutex> lock(m_mutex);
        if (m_packets.size() < maxLimit) {
            m_packets.push_back(packet);
            break;
        }else
            m_decodeThreadCond.wait(lock, [&](){
                return m_packets.size() < maxLimit;
            });
    }
}
    
AVPacket *AVDecodeThread::Pop()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_packets.empty()) return nullptr;
    AVPacket *packet = m_packets.front();
    m_packets.pop_front();
    return packet;
}
    
}
