//
//  AVPlayerProtocol.hpp
//  ffmpeg_demo
//
//  Created by yuxueqing on 2018/9/18.
//  Copyright © 2018年 yuxueqing. All rights reserved.
//

#ifndef AVPlayerProtocol_hpp
#define AVPlayerProtocol_hpp

extern "C"{
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavformat/avformat.h>
}
#include <memory>

namespace AVVideoPlayer {
  
    class AVVideoPLay
    {
    public:
        virtual void Init(long width, long height) = 0;
        virtual void Repaint(std::shared_ptr<AVFrame> frame) = 0;
    };
    
    class AVAudioPlay
    {
    public:
        int sampleRate = 0;
        int sampleSize = 0;
        int channels = 0;
        
        virtual bool Open() = 0;
        virtual void Close() = 0;
        virtual void Clear() = 0;
        
        virtual long long GetNoPlayMs() = 0;
        virtual bool Write(const unsigned char* data, int size) = 0;
        virtual int GetFree() = 0;
        virtual void SetPause(bool isPause) = 0;
        
        static AVAudioPlay *Instance();
        AVAudioPlay();
        virtual ~AVAudioPlay();
    };
    
    class AVVideoPlayerProtocol
    {
    public:
        virtual void PlayerStart() = 0;
        virtual void PlayerPrepare() = 0;
        virtual void PlayerReusme() = 0;
        virtual void PlayerStop() = 0;
    };
}

#endif /* AVPlayerProtocol_hpp */
