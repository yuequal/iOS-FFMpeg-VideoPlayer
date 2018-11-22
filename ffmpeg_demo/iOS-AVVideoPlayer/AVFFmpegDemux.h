//
//  AVFFmpegDemux.h
//  ffmpeg_demo
//
//  Created by yuxueqing on 2018/11/21.
//  Copyright © 2018年 yuxueqing. All rights reserved.
//

#import <Foundation/Foundation.h>
#include <mutex>
#include <thread>
extern "C"{
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavformat/avformat.h>
}

namespace AVVideoPlayer {
    
enum class AVPacketType { AVPacketTypeAudio, AVPacketTypeVideo,AVPacketTypeNone };
    
class AVFFmpegDemuxImpl {
    
public:
    
    virtual ~AVFFmpegDemuxImpl() = default;
    
    // 读取一帧音频或视频数据帧
    virtual int32_t Read(AVPacket *packet,int flags) = 0;
    
    // 判断数据帧类型
    virtual AVPacketType PacketType(AVPacket *packet) = 0;
    
    // set pts and dts
    virtual void SetPts(AVPacket *packet) = 0;
};

class AVFFmpegDemux : public AVFFmpegDemuxImpl {
    
public:
    // 创建一个demux分流实例，返回unique指针
    static std::unique_ptr<AVFFmpegDemuxImpl> CreateDemux(AVFormatContext *formatContext);
    
    virtual int32_t Read(AVPacket *packet, int flags);
    
    virtual AVPacketType PacketType(AVPacket *packet);
    
    virtual void SetPts(AVPacket *packet);
    
private:
    int m_videoIndex;
    int m_audioIndex;
    long long m_totalMills;
    
    std::mutex m_mutex;
    AVFormatContext *m_formatContext;
};

}
