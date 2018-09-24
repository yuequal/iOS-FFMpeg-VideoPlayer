//
//  AVAudioThread.hpp
//  ffmpeg_demo
//
//  Created by yuxueqing on 2018/9/20.
//  Copyright © 2018年 yuxueqing. All rights reserved.
//

#ifndef AVAudioThread_hpp
#define AVAudioThread_hpp

#include "AVDecodeThread.hpp"
#include "AVPlayerProtocol.hpp"
#include <mutex>
namespace AVVideoPlayer {
    
    class AVAudioThread : public AVDecodeThread
    {
    public:
        AVAudioThread(AVCodecParameters *params, AVVideoCall *call, const AVDecodePtr& ptr, const int& maxSize);
        virtual ~AVAudioThread();
        
    public:
        virtual bool Open(int sampleRate, int size);
        
        void SetPause(bool isPause);
        void StartAudioThread();
        
    public:
        std::atomic<bool> m_isPause;
        long long pts = 0;
    private:
        std::mutex m_audioMutex;
        AVVideoCall *m_call = nullptr;
        AVCodecParameters *m_params = nullptr;
    };
}
    
    
#endif /* AVAudioThread_hpp */
