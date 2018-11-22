//
//  AVFFmpegDemux.m
//  ffmpeg_demo
//
//  Created by yuxueqing on 2018/11/21.
//  Copyright © 2018年 yuxueqing. All rights reserved.
//

#import "AVFFmpegDemux.h"

namespace AVVideoPlayer {
    
int32_t AVFFmpegDemux::Read(AVPacket *packet, int flags)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (!m_formatContext) return -1;
    if (!packet) packet = av_packet_alloc();
    
    int ret = av_read_frame(m_formatContext, packet);
    if (ret != 0) {
        av_packet_unref(packet);
        av_packet_free(&packet);
        
    }
    //set pts and dts
    
    return 0;
}
    
AVPacketType  AVFFmpegDemux::PacketType(AVPacket *packet)
{
    return AVPacketType::AVPacketTypeVideo;
}
    
void AVFFmpegDemux::SetPts(AVPacket *packet)
{
    
}
    
}
