//
//  AVVideoPlayerInternal.h
//  ffmpeg_demo
//
//  Created by yuxueqing on 2018/9/30.
//  Copyright © 2018年 yuxueqing. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "AVVideoPlayer.hpp"

namespace AVVideoPlayer {
    
class AVVideoPlayerInternal;
    
class AVVideoPlayerSystemListener {
    
public:
    class AVApplicationListener {
    public:
        virtual ~AVApplicationListener() = default;
        virtual void ApplicationWillResign() = 0;
        virtual void ApplicationWillEnterBackground() = 0;
        virtual void ApplicationDidBecomeActive() = 0;
    };
    
    static std::unique_ptr<AVVideoPlayerSystemListener> Create(AVApplicationListener& lisenter);
    
    AVApplicationListener& Listener() const;
    
    virtual ~AVVideoPlayerSystemListener() = default;
    
protected:
    AVVideoPlayerSystemListener(AVApplicationListener& listener);
    
    AVApplicationListener& m_listener;
};

class AVVideoPlayerListener : public std::enable_shared_from_this<AVVideoPlayerListener> , public AVVideoPlayerSystemListener
{
    
protected:
    AVVideoPlayerListener(AVApplicationListener& listener);
    virtual ~AVVideoPlayerListener();
    
    std::shared_ptr<AVVideoPlayerListener> SharedListener();
    
    friend std::unique_ptr<AVVideoPlayerSystemListener> AVVideoPlayerSystemListener::Create(AVApplicationListener& alistener);
private:
    id m_willResignObserver ;
    id m_willEnterBackgroundObserver;
    id m_didBecomeActiveObserver;
};

class AVPlayerNotificationListener : public AVVideoPlayerListener::AVApplicationListener
{
    
public:
    AVPlayerNotificationListener();
    virtual ~AVPlayerNotificationListener();
    
    virtual void ApplicationWillResign();
    virtual void ApplicationWillEnterBackground();
    virtual void ApplicationDidBecomeActive();
    
private:
    std::weak_ptr<AVVideoPlayerInternal> m_videoPrivate;
};

class AVVideoPlayerInternal {
    
public:
    AVVideoPlayerInternal(AVVideoPlayer* videoPlayer);
    
    virtual void Destroyed();
    
    virtual void Pause();
    
    virtual void Stop();
    
    virtual void Resume();
    
    virtual void Restart();
    
    virtual void Prepare();
    
    virtual void Seek(double pos);
    
private:
    AVVideoPlayer *m_videoPlayer;
    
};
}
