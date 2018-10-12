//
//  AVDemux.cpp
//  ffmpeg_demo
//
//  Created by yuxueqing on 2018/9/7.
//  Copyright © 2018年 yuxueqing. All rights reserved.
//

#include "AVDemux.hpp"
#include <iostream>

namespace AVVideoPlayer
{
    
static double r2d(AVRational r)
{
    return r.den == 0 ? 0 : (double)r.num / (double)r.den;
}

AVDemux::AVDemux(AVFormatContext *formatContext)
{
    static std::once_flag onceFlag;
    std::call_once(
        onceFlag,
        [this,formatContext]()
    {
        av_register_all();
        avformat_network_init();
        m_formatContex = formatContext;
    });
}

AVDemux::~AVDemux()
{
    
}
    
void AVDemux::Start()
{
    m_demuxThread = std::thread([this](){
        pthread_setname_np("com.videoplayer.demux");
     //   AVCodecParameters *paras =  this->CopyVideoParams();
    });
    
}

bool AVDemux::Open(const char *url)
{
    Close();
    av_register_all();
    avformat_network_init();
    m_mutex.lock();
    int ret = avformat_open_input(&m_formatContex, url, 0, 0);
    if (ret != 0) {
        m_mutex.unlock();
        char buff[1024] = { 0 };
        av_strerror(ret, buff, sizeof(buff) - 1);
        return false;
    }

    ret = avformat_find_stream_info(m_formatContex, 0);
    this->totalMills = m_formatContex->duration / (AV_TIME_BASE / 1000);
    
    av_dump_format(m_formatContex, 0, url, 0);
    videoStream = av_find_best_stream(m_formatContex, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
    AVStream *stream = m_formatContex->streams[videoStream];
    
    this->width = stream->codecpar->width;
    this->height = stream->codecpar->height;
    
    audioStream = av_find_best_stream(m_formatContex, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
    stream = m_formatContex->streams[audioStream];
    
    
    m_mutex.unlock();
    
    return true;
}

AVPacket* AVDemux::Read()
{
    m_mutex.lock();
    if (!m_formatContex) {
        m_mutex.unlock();
        return NULL;
    }
    AVPacket *packet = av_packet_alloc();
    
    int ret = av_read_frame(m_formatContex, packet);
    m_mutex.unlock();
    if (ret != 0) {
        av_packet_free(&packet);
        return NULL;
    }
    //set pts and dts
    packet->pts = packet->pts * (1000 * (r2d(m_formatContex->streams[packet->stream_index]->time_base)));
    packet->dts = packet->dts * (1000 * (r2d(m_formatContex->streams[packet->stream_index]->time_base)));
    std::cout << packet->pts << "  " <<std::endl;
    return packet;
}

bool AVDemux::IsAudio(AVPacket *packet)
{
    if (!packet) return false;
    if (packet->stream_index == audioStream) return true;
    return false;
}

AVCodecParameters* AVDemux::CopyAudioParams()
{
    m_mutex.lock();
    if (!m_formatContex) {
        m_mutex.unlock();
        return NULL;
    }
    AVCodecParameters *params = avcodec_parameters_alloc();
    int ret = avcodec_parameters_copy(params, m_formatContex->streams[audioStream]->codecpar);
    if (ret < 0) {
        m_mutex.unlock();
        avcodec_parameters_free(&params);
        return NULL;
    }
    m_mutex.unlock();
    return params;
}

AVCodecParameters* AVDemux::CopyVideoParams()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (!m_formatContex) return nullptr;
    AVCodecParameters *paras = avcodec_parameters_alloc();
    AVCodecParameters *sp = m_formatContex->streams[videoStream]->codecpar;
    int ret = avcodec_parameters_copy(paras, static_cast<const AVCodecParameters *>(sp));
    if (ret < 0) {
        avcodec_parameters_free(&paras);
        return NULL;
    }
    return paras;
}
    
AVFormatContext * AVDemux::FormatContext() const
{
    return m_formatContex;
}

int AVDemux::VStreamIndex() const
{
    return videoStream;
}

void AVDemux::Clear()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (!m_formatContex) return;
    avformat_flush(m_formatContex);
}

void AVDemux::Close()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (!m_formatContex) return;
    avformat_close_input(&m_formatContex);
}

bool AVDemux::Seek(double pos)
{
    return false;
}
}
