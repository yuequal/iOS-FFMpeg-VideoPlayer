//
//  AVVideoPlayer.cpp
//  ffmpeg_demo
//
//  Created by yuxueqing on 2018/9/18.
//  Copyright © 2018年 yuxueqing. All rights reserved.
//

#include "AVVideoPlayer.hpp"

namespace AVVideoPlayer {
    
AVVideoPlayer::AVVideoPlayer()
{
    m_decode = std::make_shared<AVDecode>();
}
AVVideoPlayer::~AVVideoPlayer()
{
    
}

double AVVideoPlayer::Duration() const
{
    return m_duration;
}

void AVVideoPlayer::SeekToTime(double pos)
{
    
}

bool AVVideoPlayer::Open(const char *url)
{
    av_register_all();
    avformat_network_init();
    AVFormatContext *context = avformat_alloc_context();
    if (avformat_open_input(&context, url, nullptr, nullptr) != 0) {
        m_demux = std::make_shared<AVDemux>(context);
        m_demux->Open(url);
        AVPacket *pkt = m_demux->Read();
        m_decode->Open(m_demux->CopyVideoParams());
        m_decode->Send(pkt);
        AVModeFrame *frame =  m_decode->RecvFrame();
        if (m_videoPlay) {
            m_videoPlay->Repaint(frame->Frame());
        }
    }
    
    return true;
}
void AVVideoPlayer::Play()
{
    
}

void AVVideoPlayer::SetPause(bool pause)
{
    
}

void AVVideoPlayer::Prepare()
{
    
}
void AVVideoPlayer::Loading()
{
    
}
void AVVideoPlayer::Restart()
{
    
}
    
void AVVideoPlayer::Stop()
{
    
}
    
}
