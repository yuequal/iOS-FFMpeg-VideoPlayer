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
#include <thread>
namespace AVVideoPlayer {

    using AVDemuxCallBack = std::function<int(int, void *)>;

    class AVDemuxThread {

    public:
        AVDemuxThread();
        virtual ~AVDemuxThread();

        void StartDemux(const char *url, const AVDemuxCallBack& cb, void *context);

    public:
        virtual bool Open(const char *url, AVVideoCall* call);

        virtual void Start();
        virtual void Clear();
        virtual void Seek(double pos);

        void DemuxThread();
        void SetPause(bool isPause);

    public:
        std::atomic<bool> m_isExit;
        std::atomic<bool> m_isPause;
        long long m_pts = 0;
        long long m_totalMills = 0;

    protected:
        std::mutex m_mutex;
        std::thread m_demuxThread;
        std::unique_ptr<AVDemux> m_demux;
    };
}

#endif /* AVDemuxThread_hpp */
