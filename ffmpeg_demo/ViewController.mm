//
//  ViewController.m
//  ffmpeg_demo
//
//  Created by yuxueqing on 2018/9/7.
//  Copyright © 2018年 yuxueqing. All rights reserved.
//

#import "ViewController.h"
#import "AVDemux.hpp"
#import "AVDemuxThread.hpp"
#import "AVDecode.hpp"
#import <iostream>
#import "AVViewController.h"
#import "AVVideoRender.h"
#import "AVDispatchQueue.h"
#import "AVVideoImage.h"

@interface AVVideoFrameRenderProvider : NSObject <AVVideoRender>
{
    AVVideoFrame *m_videoFrame;
    UIImage *m_videoImage;
    CGSize m_videoSize;
}
@end

@implementation AVVideoFrameRenderProvider

- (void)setFrameSize:(CGSize)size
{
    if (!CGSizeEqualToSize(size, m_videoSize)) {
        m_videoSize = size;
    }
}
- (void)renderFrame:(AVVideoFrame *)videoFrame{
}

@end

@interface ViewController ()
{
    std::thread m_decodeThread;
    std::thread m_readFrameThread;
    struct SwsContext *m_swsContextt;
    __strong id<AVVideoRender> m_renderProtcol;
    __strong UIImageView *m_imageView;
    std::shared_ptr<AVVideoPlayer::AVDemux> m_demux;
    std::shared_ptr<AVVideoPlayer::AVDemuxThread> m_demuxThread;
    std::shared_ptr<AVVideoPlayer::AVDecode> m_decode;
    std::shared_ptr<AVVideoPlayer::AVVideoFrameRender> m_videoRender;
    std::shared_ptr<AVVideoPlayer::AVVideoFrameImage> m_frameImage;
}
@end

@implementation ViewController

- (void)dealloc
{
    m_demux.reset();
    m_demuxThread.reset();
    m_decode.reset();
    m_videoRender.reset();
    m_frameImage.reset();
    sws_freeContext(m_swsContextt);
    m_swsContextt = NULL;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    [self setupImageView];
    const char *file = [[[NSBundle mainBundle] pathForResource:@"video" ofType:@"mp4"] UTF8String];
    m_renderProtcol = [AVVideoFrameRenderProvider new];
    AVFormatContext *ctx = avformat_alloc_context();
    m_demux = std::make_shared<AVVideoPlayer::AVDemux>(ctx);
    m_videoRender = std::make_shared<AVVideoPlayer::AVVideoFrameRender>(m_renderProtcol);
    bool success = m_demux->Open(file);
    if (success) {
        AVPacket* packet = m_demux->Read();
        std::cout << "-- size: "<<packet->size << " -- pts: "<< packet->pts << std::endl;
      //  av_packet_free(&packet);
    }

    m_demux->Start();
    AVVideoPlayer::AVDecode decode;
    m_decode = std::shared_ptr<AVVideoPlayer::AVDecode>(new AVVideoPlayer::AVDecode());
    AVCodecParameters *paras = m_demux->CopyVideoParams();
    success = m_decode->Open(paras);
    m_swsContextt = sws_alloc_context();
    m_frameImage = std::make_shared<AVVideoPlayer::AVVideoFrameImage>(m_swsContextt,m_decode->codecContext());
    m_decodeThread = std::thread([self](){
        while (true) {
            @autoreleasepool {
                
            AVPacket *packet = m_demux->Read();
            if (!packet) continue;
            std::cout<< packet->size << packet->pts << std::endl;
            if (m_demux->IsAudio(packet)) continue;
            bool success = m_decode->Send(packet);
            if (!success) continue;
            AVVideoPlayer::AVModeFrame *modeFrame = m_decode->RecvFrame();
            if (!modeFrame) continue;
            std::shared_ptr<AVFrame> frame = modeFrame->Frame();
            if (!frame) continue;
            if (m_videoRender) {
                m_videoRender->RenderFrame(static_cast<const AVVideoPlayer::AVModeFrame *>(modeFrame));
            }
            unsigned char *buffer = (unsigned char *)malloc(320 * 250 * 4 + 1);
            success = m_frameImage->Open(static_cast<const AVVideoPlayer::AVModeFrame *>(modeFrame), buffer, 320, 250);
            UIImage *videoImage = m_frameImage->FrameImage(static_cast<const unsigned char*>(buffer), 320, 250);
            if (videoImage) {
                [AVDispatchQueue dispatchTaskAsyncOnMainQueue:^{
                    self->m_imageView.image = videoImage;
                }];
            }
            std::cout << frame->linesize[0] << "--- size :" << frame->pkt_size << " pts :"<< frame->pts << "  dts:" << frame->pkt_dts << std::endl;
        }
        }
    });

}

- (void)setupImageView
{
    m_imageView = [[UIImageView alloc] initWithFrame:CGRectMake(40, 100, 320, 250)];
    m_imageView.backgroundColor = [UIColor clearColor];
    [self.view addSubview:m_imageView];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


@end
