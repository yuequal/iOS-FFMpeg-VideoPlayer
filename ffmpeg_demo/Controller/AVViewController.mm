//
//  AVViewController.m
//  ffmpeg_demo
//
//  Created by yuxueqing on 2018/9/9.
//  Copyright © 2018年 yuxueqing. All rights reserved.
//

#import "AVViewController.h"
//#import "AVDemuxThread.hpp"
#import "AVVideoPlayer.hpp"
#import <objc/runtime.h>
#import "AVDispatchQueue.h"
#import "AVVideoRender.h"

static inline void AVPostNotification(NSString *name, id object, id userInfo)
{
    if (pthread_main_np())
        [[NSNotificationCenter defaultCenter] postNotificationName:name object:object userInfo:userInfo];
    else
        dispatch_async(dispatch_get_main_queue(), ^{
            [[NSNotificationCenter defaultCenter] postNotificationName:name object:object userInfo:userInfo];
        });
}

static inline id AVCallDelegate(AVViewController *self, id delegate, SEL selector, int bitRate)
{
    if (!delegate || ![delegate respondsToSelector:selector]) return nil;
    typedef id (*AVCallMethodImp)(id, SEL, AVViewController *, int);
    AVCallMethodImp method = (AVCallMethodImp)[delegate methodForSelector:selector];
    return reinterpret_cast<AVCallMethodImp>(method)(self,selector,delegate,bitRate);
}

static inline id AVCallBackDelegate(AVViewController *self, id delegate, SEL selector, int bitRate, int frameCount)
{
    if (!delegate || ![delegate respondsToSelector:selector]) return nil;
    typedef id (*AVCallBackMethodImp)(id, SEL, AVViewController *, int, int);
    AVCallBackMethodImp method = (AVCallBackMethodImp)[delegate methodForSelector:selector];
    return reinterpret_cast<AVCallBackMethodImp>(method)(self,selector,delegate,bitRate,frameCount);
}

@interface AVViewController ()
{
@private
    __strong id<AVVideoRender> m_renderProtocol;
    __weak id<AVViewControllerDelegate> m_delegate;
    std::shared_ptr<AVVideoPlayer::AVDemuxThread> m_demuxThread;
    std::shared_ptr<AVVideoPlayer::AVVideoPlayer> m_videoPlayer;
    std::unique_ptr<AVVideoPlayer::AVVideoFrameRender> m_videoRender;
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
    auto callback = [=](int bitrate, void *context) -> void {
        if (!context) return ;
        AVViewController *vc = (__bridge AVViewController *)context;
        SEL selector = @selector(AVideoPlayerControllerTime:bitRate:);
        AVCallDelegate(vc, vc->m_delegate, selector, bitrate);
    };
    m_demuxThread->StartDemux(filePath,callback,(__bridge void *)self);
    AVPostNotification(AVVideoPlayerDidBeginEditingNotification, self->m_delegate, nil);
}

- (void)setPause:(BOOL)isPause
{
   self->m_videoPlayer->SetPause(isPause);
}

- (void)seekTime:(double)time
{
   self->m_videoPlayer->SeekToTime(time);
}

- (void)prepareToPlay
{
    self->m_videoPlayer->Prepare();
}

- (void)restart
{
    self->m_videoPlayer->Restart();
}

- (void)openUrl:(NSString *)url
{
    const char *filePath = [url UTF8String];
    self->m_videoPlayer->Open(filePath);
}

- (void)_registerForDelegateNotification:(NSString *)name seletor:(SEL)aSelecot
{
    NSNotificationCenter *defaultCenter = [NSNotificationCenter defaultCenter];
    if ([self->m_delegate respondsToSelector:aSelecot])
    [defaultCenter addObserver:self->m_delegate selector:aSelecot name:name object:self];
}

- (void)setDelegate:(id<AVViewControllerDelegate>)delegate
{
    if (self->m_delegate == delegate)
        return;
    
    NSNotificationCenter *defaultCenter = [NSNotificationCenter defaultCenter];
   
    [defaultCenter removeObserver:self->m_delegate name:AVVideoPlayerDidChangeNotification object:nil];
    [defaultCenter removeObserver:self->m_delegate name:AVVideoPlayerWillResignNotification object:nil];
    [defaultCenter removeObserver:self->m_delegate name:AVVideoPlayerDidBeginEditingNotification object:nil];
    
    self->m_delegate = delegate;
    
    [self _registerForDelegateNotification:AVVideoPlayerDidBeginEditingNotification seletor:@selector(AVideoPlayerControllerTime:bitRate:)];
    [self _registerForDelegateNotification:AVVideoPlayerWillResignNotification seletor:@selector(AVideoPlayerControllerTime:bitRate:)];
}

- (id<AVViewControllerDelegate>)videoDelegate
{
    return self->m_delegate;
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
