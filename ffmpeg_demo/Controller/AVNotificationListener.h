//
//  AVNotificationListener.h
//  ffmpeg_demo
//
//  Created by yuxueqing on 2018/10/9.
//  Copyright © 2018年 yuxueqing. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "AVVideoPlayerInternal.h"
namespace AVVideoPlayer {
    
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
};

}
