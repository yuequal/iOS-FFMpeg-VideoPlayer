//
//  AVAudioRender.m
//  ffmpeg_demo
//
//  Created by yuxueqing on 2018/10/18.
//  Copyright © 2018年 yuxueqing. All rights reserved.
//

#import "AVAudioRender.h"
#import <mutex>

namespace AVVideoPlayer {
    
AVAudioRender::AVAudioRender(id<AVAudioDataProtocol> protocol ,float sampleRates)
: m_dataProtocol(protocol)
, m_sampleRate(sampleRates)
{
    std::shared_ptr<AVAudioRender> render = shared_from_this();
    AVAudioNotificationObserver *observer = [[AVAudioNotificationObserver alloc] initWithAudioRender:render];
    m_observer = observer;
}
    
AVAudioRender* AVAudioRender::Instance(id<AVAudioDataProtocol> protocol, float sampleRates)
{
    static std::once_flag onceFlag;
    static AVAudioRender *render;
    std::call_once(onceFlag,[protocol,sampleRates]()
    {
        render = new AVAudioRender(protocol,sampleRates);
    });
    return render;
}
    
std::unique_ptr<AVAudioRenderImpl> AVAudioRender::Create(id<AVAudioDataProtocol> protocol, float sampleRates)
{
    return std::unique_ptr<AVAudioRender>(new AVAudioRender(protocol,sampleRates));
}

AVAudioRender::~AVAudioRender()
{
    
}

void AVAudioRender::Start()
{
    
}

void AVAudioRender::Stop()
{
    
}

bool AVAudioRender::IsPlaying()
{
    return m_isPlaying;
}

float AVAudioRender::SampleRates() const
{
    return m_sampleRate;
}
    
int AVAudioRender::Channels() const
{
    return m_channels;
}

void AVAudioRender::SetIsPlaying(bool isPlaying)
{
    m_isPlaying = isPlaying;
}

void AVAudioRender::SetSampleRate(float sampleRates)
{
    m_sampleRate = sampleRates;
}
    
void AVAudioRender::SetChnnels(int channels)
{
    m_channels = channels;
}
    
float AVAudioRenderImpl::HWSamplerates()
{
    return 0;
}

void AVAudioRender::HandleNotification(NSNotification *notification)
{
    
}
    
}

@implementation AVAudioNotificationObserver

- (void)dealloc
{
    [self _removeInteruptNotification];
}

- (instancetype)initWithAudioRender:(const std::shared_ptr<AVVideoPlayer::AVAudioRender> &)render
{
    self = [super init];
    if (self) {
        m_audioRender = render;
        [self _registerInteruptNotification];
    }
    return self;
}

- (void)_registerInteruptNotification
{
    [[NSNotificationCenter defaultCenter] addObserverForName:@"AVAudioSessionInterruptionNotification" object:nil queue:nil usingBlock:^(NSNotification * _Nonnull note) {
        auto audioRender = self->m_audioRender.lock();
        if (audioRender)
            audioRender->HandleNotification(note);
    }];
}

- (void)_removeInteruptNotification
{
    [[NSNotificationCenter defaultCenter] removeObserver:self name:@"AVAudioSessionInterruptionNotification" object:nil];
}

@end

