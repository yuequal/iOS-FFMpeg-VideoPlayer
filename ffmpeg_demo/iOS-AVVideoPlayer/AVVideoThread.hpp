//
//  AVVideoThread.hpp
//  ffmpeg_demo
//
//  Created by yuxueqing on 2018/9/20.
//  Copyright © 2018年 yuxueqing. All rights reserved.
//

#ifndef AVVideoThread_hpp
#define AVVideoThread_hpp

#include "AVDecodeThread.hpp"
#include "AVPlayerProtocol.hpp"
#include <mutex>
namespace AVVideoPlayer {
    class AVVideoThread : public AVDecodeThread
    {
        
    public:
        AVVideoThread(AVCodecParameters* params, AVVideoPLay* call, const AVDecodePtr& ptr, const int& maxSize);
        virtual ~AVVideoThread();
    public:
        virtual bool Open(int width, int height);
        
        void Start();
        void StartVideoThread();
        void SetPause(bool isPause);
        
    public:
        std::atomic<bool> m_isPause;
        long long m_synpts = 0;
    private:
        std::mutex m_videoMutex;
        AVVideoPLay *m_videoPlay = nullptr;
        AVCodecParameters *m_params = nullptr;
    };
}

#endif /* AVVideoThread_hpp */
