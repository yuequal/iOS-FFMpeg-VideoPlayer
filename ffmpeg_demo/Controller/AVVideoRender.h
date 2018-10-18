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
#import "AVVideoImage.h"

@protocol AVVideoRender;

namespace AVVideoPlayer {
    
template <typename VideoFrame>
class AVVideoRenderImpl
{
public:
    virtual ~AVVideoRenderImpl() = default;
    
    virtual void RenderFrame(const VideoFrame *frame) = 0;
    
    virtual void DestroyRender() = 0;
};

class AVVideoFrameRender : public AVVideoRenderImpl<AVModeFrame>
{
public:
    AVVideoFrameRender(id<AVVideoRender> render);
    
    void RenderFrame(const AVModeFrame* frame);
    
    bool Open(const AVVideoFrameImage *image);
    
    void DestroyRender();
    
public:
    std::unique_ptr<AVVideoRenderImpl<AVModeFrame>> Create(id<AVVideoRender> render,const AVVideoFrameImage *image);
    
private:
    CGSize m_renderSize;
    
    id<AVVideoRender> m_render;
    
    AVVideoFrameImage *m_frameImage;
};
}

@interface AVVideoFrame : NSObject

@property(nonatomic, readonly) int width;

@property(nonatomic, readonly) int height;

@property(nonatomic, readonly) int64_t timeStamp;

@property(nonatomic, readonly) const uint8_t *lumaY;

@property(nonatomic, readonly) const uint8_t *lumaU;

@property(nonatomic, readonly) const uint8_t *lumaV;

- (instancetype)init NS_UNAVAILABLE;
- (instancetype)new NS_UNAVAILABLE;

- (instancetype)initWithAVFrame:(const AVFrame *)frame;

- (instancetype)initWithModeFrame:(const AVVideoPlayer::AVModeFrame& )modeFrame;

- (instancetype)initWidth:(int)scaledWidth Height:(int)scaledHeight timeStamp:(int64_t)timeStamp;
@end

@protocol AVVideoRender <NSObject>

- (void)setFrameSize:(CGSize)size;

- (void)renderFrame:(AVVideoFrame *)videoFrame;

- (void)rederModeFrame:(const AVVideoPlayer::AVModeFrame *)modeFrame;

- (UIImage *)createVideoImage:(const AVVideoPlayer::AVModeFrame *)modeFrame;

@end

@protocol AVVideoViewDelegate

- (void)videoView:(id<AVVideoRender>)videoView didChangeSize:(CGSize)size;

@end
