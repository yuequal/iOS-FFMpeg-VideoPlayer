//
//  AVVideoRender.h
//  ffmpeg_demo
//
//  Created by yuxueqing on 2018/10/16.
//  Copyright © 2018年 yuxueqing. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "AVModeFrame.hpp"

@protocol AVVideoRender;

namespace AVVideoPlayer {
    
template <typename VideoFrame>
class AVVideoRenderImpl
{
public:
    virtual ~AVVideoRenderImpl() = default;
    
    virtual void InitFrame(const VideoFrame& frame) = 0;
    
    virtual void Destroy() = 0;
};

class AVVideoFrameRender : public AVVideoRenderImpl<AVModeFrame>
{
public:
    AVVideoFrameRender(id<AVVideoRender> render);
    
    void InitFrame(const AVModeFrame& frame);
    
    void Destroy();
    
public:
    std::unique_ptr<AVVideoRenderImpl<AVModeFrame>> Create(id<AVVideoRender> render);
    
private:
    CGSize m_renderSize;
    
    id<AVVideoRender> m_render;
};
}

@interface AVVideoFrame : NSObject

@property(nonatomic, readonly) int width;

@property(nonatomic, readonly) int height;

@property(nonatomic, readonly) int64_t timeStamp;

- (instancetype)init NS_UNAVAILABLE;
- (instancetype) new NS_UNAVAILABLE;

- (instancetype)initWithModeFrame:(const AVVideoPlayer::AVModeFrame& )modeFrame;

- (instancetype)initWidth:(int)scaledWidth Height:(int)scaledHeight timeStamp:(int64_t)timeStamp;
@end

@protocol AVVideoRender <NSObject>

- (void)setFrameSize:(CGSize)size;

- (void)renderFrame:(AVVideoFrame *)videoFrame;

@end

@protocol AVVideoViewDelegate

- (void)videoView:(id<AVVideoRender>)videoView didChangeSize:(CGSize)size;

@end
