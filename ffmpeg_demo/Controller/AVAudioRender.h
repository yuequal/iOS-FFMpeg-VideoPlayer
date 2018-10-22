//
//  AVAudioRender.h
//  ffmpeg_demo
//
//  Created by yuxueqing on 2018/10/18.
//  Copyright © 2018年 yuxueqing. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AudioToolbox/AudioToolbox.h>
#import "AVModeFrame.hpp"

@class AVAudioNotificationObserver;
@protocol AVAudioDataProtocol;

namespace AVVideoPlayer {
    
class AVAudioRenderImpl
{
public:
    static std::unique_ptr<AVAudioRenderImpl> Create(id<AVAudioDataProtocol> protocol,float sampleRates);
    
    virtual ~AVAudioRenderImpl() = default;
    
    virtual void Start() = 0;
    virtual void Stop() = 0;
    virtual bool IsPlaying() = 0;
    
    virtual float SampleRates() const = 0;
    virtual int Channels() const = 0;
public:
    
    static float HWSamplerates();
    
    static unsigned long MaxChannels();
};

class AVAudioRender  : public AVAudioRenderImpl , public std::enable_shared_from_this<AVAudioRender>
{
public:
    AVAudioRender(id<AVAudioDataProtocol> protocol, float sampleRates);
    virtual ~AVAudioRender();
    
public:
    
    virtual void Start();
    virtual void Stop();
    virtual bool IsPlaying();
    virtual float SampleRates() const;
    virtual int Channels() const;
    
    virtual void SetIsPlaying(bool);
    
    friend float AVAudioRenderImpl::HWSamplerates();
    
public:
    void SetSampleRate(float sampleRate);
    void SetChnnels(int channels);
    
public:
    void HandleNotification(NSNotification *notification);
    
    static std::unique_ptr<AVAudioRenderImpl> Create(id<AVAudioDataProtocol> protocol,float sampleRates);
    
    static AVAudioRender *Instance(id<AVAudioDataProtocol> protocol, float sampleRtaes);
    
public:
    static OSStatus RenderData(void *userData, AudioBufferList *dataBuffer,AudioUnitRenderActionFlags *flags,const AudioTimeStamp *timeStamp ,UInt32 element, UInt32 frames);
    
private:
    
    float m_sampleRate { 0 };
    int m_channels { 0 };
    bool m_isPlaying { false };
    SInt16 *m_outData;
    
    AUGraph *m_audioGraph ;
    AUNode *m_audioNode;
    AudioUnit *m_audioUint;
    AUNode *m_convertNode;
    AudioUnit *m_convertUint;
    
    id<AVAudioDataProtocol> m_dataProtocol;
    AVAudioNotificationObserver *m_observer;
};
}

@protocol AVAudioDataProtocol <NSObject>

- (NSInteger)audioData:(SInt16*)sampleBuffer frames:(NSInteger)frames channels:(NSInteger)channels;

@end

@interface AVAudioNotificationObserver : NSObject
{
    std::weak_ptr<AVVideoPlayer::AVAudioRender> m_audioRender;
}

- (instancetype)initWithAudioRender:(const std::shared_ptr<AVVideoPlayer::AVAudioRender>&)render;

@end

