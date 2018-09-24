//
//  AVDecode.cpp
//  ffmpeg_demo
//
//  Created by yuxueqing on 2018/9/9.
//  Copyright © 2018年 yuxueqing. All rights reserved.
//

#include "AVDecode.hpp"

void AVFreePacket(AVPacket **packet)
{
    if (packet || (*packet)) return;
    av_packet_free(packet);
}

namespace AVVideoPlayer {

void AVDecode::Close()
{
    m_mutex.lock();
    if (m_codecContext) {
        avcodec_close(m_codecContext);
        avcodec_free_context(&m_codecContext);
    }
    pts = 0;
    m_mutex.unlock();
}

void AVDecode::Clear()
{
    m_mutex.lock();
    if (m_codecContext)
        avcodec_flush_buffers(m_codecContext);
    m_mutex.unlock();
}
    
bool AVDecode::Open(AVCodecParameters *para)
{
    if (!para) return false;
    Close();
    
    AVCodec *vcodec = avcodec_find_decoder(para->codec_id);
    if (!vcodec) {
        avcodec_parameters_free(&para);
        return false;
    }
    m_mutex.lock();
    m_codecContext = avcodec_alloc_context3(vcodec);
    
    avcodec_parameters_to_context(m_codecContext, static_cast<const AVCodecParameters *>(para));
    avcodec_parameters_free(&para);
    
    m_codecContext->thread_count = 8;
    
    int ret = avcodec_open2(m_codecContext, static_cast<const AVCodec *>(vcodec), NULL);
    if (ret != 0) {
        avcodec_free_context(&m_codecContext);
        m_mutex.unlock();
        char buff[1024] = { 0 };
        av_strerror(ret, buff, sizeof(buff) - 1);
        return false;
    }
    m_mutex.unlock();
    return true;
}

bool AVDecode::Send(AVPacket *packet)
{
    if (!packet || !packet->data || packet->size <= 0) return false;
    m_mutex.lock();
    if (!m_codecContext) {
        m_mutex.unlock();
        return false;
    }
    int ret = avcodec_send_packet(m_codecContext, static_cast<const AVPacket *>(packet));
    m_mutex.unlock();
    av_packet_free(&packet);
    if (ret != 0) return false;
    return true;
}

//void AVDecode::RecvFrame(AVModeFrame& modeFrame)
//{
//}
//std::shared_ptr<AVModeFrame>& AVDecode::RecvFrame()
//{
//}
AVModeFrame* AVDecode::RecvFrame()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (!m_codecContext) return nullptr;
    
    AVFrame *frame = av_frame_alloc();
    int ret = avcodec_receive_frame(m_codecContext, frame);
    if (ret != 0) {
        av_frame_unref(frame);
        av_frame_free(&frame);
        return nullptr;
    }
    AVModeFrame *mFrame = AVModeFrame::createMFrame(frame);
    pts = frame->pts;
    frame = nullptr;
    return mFrame;
}

AVDecode::AVDecode()
{
    
}
    
AVDecode::~AVDecode()
{
    
}
    
    
}
