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

namespace AVVideoPlayer {
    
    enum class AVVideoPlayerFlag {
        AVVideoPlayerDone,
        AVVideoPlayerPending,
        AVVideoPlayerFailed
    };
    
    class AVVideoPlayer : public std::enable_shared_from_this<AVVideoPlayer>
    {
    public:
        ~AVVideoPlayer();
        AVVideoPlayerFlag Open(const char *url);
        AVVideoPlayerFlag Play();
        AVVideoPlayerFlag Pause(bool pause);
        
    };
}

#endif /* AVVideoPlayer_hpp */
