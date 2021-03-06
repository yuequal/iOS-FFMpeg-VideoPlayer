//
//  AVVideoRender.m
//  ffmpeg_demo
//
//  Created by yuxueqing on 2018/10/16.
//  Copyright © 2018年 yuxueqing. All rights reserved.
//

#import "AVVideoRender.h"
#import "AVDispatchQueue.h"

namespace AVVideoPlayer {

AVVideoFrameRender::AVVideoFrameRender(id<AVVideoRender> render)
    : m_render(render)
    , m_renderSize(CGSizeZero) {}
 
void AVVideoFrameRender::RenderFrame(const AVModeFrame* modeFrame)
{
    AVVideoFrame *videoFrame = [[AVVideoFrame alloc] initWidth:modeFrame->Width() Height:modeFrame->Height() timeStamp:modeFrame->TimeStamp()];
    CGSize frameSize = CGSizeMake(modeFrame->Width(), modeFrame->Height());
    if (!CGSizeEqualToSize(m_renderSize, frameSize)) {
        m_renderSize = frameSize;
        [m_render setFrameSize:m_renderSize];
    }
    [m_render renderFrame:videoFrame];
}
    
std::unique_ptr<AVVideoRenderImpl<AVModeFrame>> AVVideoFrameRender::Create(id<AVVideoRender> render,const AVVideoFrameImage *image)
{
    return std::unique_ptr<AVVideoFrameRender>(new AVVideoFrameRender(render));
}
    
void AVVideoFrameRender::DestroyRender() {}
    
}

@implementation AVVideoFrame

- (instancetype)initWidth:(int)width Height:(int)height timeStamp:(int64_t)timeStamp
{
    self = [super init];
    if (self) {
        _width = width;
        _height = height;
        _timeStamp = timeStamp;
    }
    return self;
}

- (instancetype)initWithModeFrame:(const AVVideoPlayer::AVModeFrame &)modeFrame
{
    return [self initWidth:modeFrame.Width()
                    Height:modeFrame.Height()
                 timeStamp:modeFrame.TimeStamp()];
}

@end
