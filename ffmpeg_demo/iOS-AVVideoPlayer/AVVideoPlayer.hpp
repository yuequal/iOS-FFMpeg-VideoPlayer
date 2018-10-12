//
//  AVVideoPlayer.hpp
//  ffmpeg_demo
//
//  Created by yuxueqing on 2018/9/18.
//  Copyright © 2018年 yuxueqing. All rights reserved.
//

#ifndef AVVideoPlayer_hpp
#define AVVideoPlayer_hpp

#include <mutex>
#include "AVDemuxThread.hpp"
namespace AVVideoPlayer {
    
    enum class AVVideoPlayerFlag {
        AVVideoPlayerDone,
        AVVideoPlayerPending,
        AVVideoPlayerFailed
    };
    
    class AVVideoPlayer : public std::enable_shared_from_this<AVVideoPlayer>
    {
    public:
        AVVideoPlayer();
        ~AVVideoPlayer();
        bool Open(const char *url);
        void Play();
        void SetPause(bool pause);
        void SeekToTime(double pos);
        void Prepare();
        void Loading();
        void Restart();
        double Duration() const;
        
    private:
        double m_duration = 0.0;
        std::shared_ptr<AVDemuxThread> m_demuxThread;
        std::shared_ptr<AVDemux> m_demux;
        std::shared_ptr<AVDecode> m_decode;
        std::unique_ptr<AVVideoPLay> m_videoPlay;
        AVPacketQueue<AVPacket> m_videoPacketQueue;
        AVPacketQueue<AVPacket> m_audioPacketQueue;
    };
}

#endif /* AVVideoPlayer_hpp */
