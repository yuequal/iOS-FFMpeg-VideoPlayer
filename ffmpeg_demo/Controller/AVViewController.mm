//
//  AVViewController.m
//  ffmpeg_demo
//
//  Created by yuxueqing on 2018/9/9.
//  Copyright © 2018年 yuxueqing. All rights reserved.
//

#import "AVViewController.h"
#import "AVDemuxThread.hpp"
@interface AVViewController ()
{
@private
    __weak id<AVViewControllerDelegate> m_delegate;
    std::shared_ptr<AVVideoPlayer::AVDemuxThread> m_demuxThread;
}
@end

@implementation AVViewController
- (instancetype)init
{
    self = [super init];
    if (self) {
     
    }
    return self;
}
- (void)viewDidLoad {
    [super viewDidLoad];
    
    const char *filePath = [[[NSBundle mainBundle] pathForResource:@"video" ofType:@"mp4"] UTF8String];
    m_demuxThread->StartDemux(filePath, [=](int bitRate, void *context) -> int {
        if (!context) return -1;
        AVViewController *vc = (__bridge AVViewController *)context;
        if (vc && [vc->m_delegate respondsToSelector:@selector(videoplayerTime:bitRate:)]) {
            [vc->m_delegate videoplayerTime:vc bitRate:bitRate];
        }
        return 0;
    }, (__bridge void *)self);
}

- (void)setPause:(BOOL)isPause
{
   self->m_demuxThread->SetPause(isPause);
}

- (void)seekTime:(double)time
{
   self->m_demuxThread->Seek(time);
}

- (void)setDelegate:(id<AVViewControllerDelegate>)delegate
{
    if (![self->m_delegate isEqual:delegate]) {
        self->m_delegate = delegate;
    }
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/

@end
