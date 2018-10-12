//
//  AVDemuxThread.hpp
//  ffmpeg_demo
//
//  Created by yuxueqing on 2018/9/11.
//  Copyright © 2018年 yuxueqing. All rights reserved.
//

#ifndef AVDemuxThread_hpp
#define AVDemuxThread_hpp

#include "AVDemux.hpp"
#include "AVPlayerProtocol.hpp"
#include "AVVideoThread.hpp"
#include <thread>
namespace AVVideoPlayer {

    using AVDemuxCallBack = std::function<void(int, void *)>;

    class AVDemuxThread {

    public:
        AVDemuxThread();
        virtual ~AVDemuxThread();

        void StartDemux(const char *url, const AVDemuxCallBack& cb, void *context);

    public:
        virtual bool Open(const char *url, AVVideoPLay* call);

        virtual void Start();
        virtual void Clear();
        virtual void Seek(double pos);

        void DemuxThread();
        void SetPause(bool isPause);
        void SetThreadName(const char *name);

    public:
        enum class AVState { AVSleep, AVRun, AVPause };
        std::atomic<AVState> m_state { AVState::AVPause };
    
        long long m_pts = 0;
        long long m_totalMills = 0;
 
    protected:
        const char *m_fileUrl = 0;
        AVVideoPLay *m_videoPlay = 0;
        std::mutex m_mutex;
        std::thread m_demuxThread;
        std::shared_ptr<AVDemux> m_demux;
        std::shared_ptr<AVVideoThread> m_videoThread;
    };
}

#endif /* AVDemuxThread_hpp */
