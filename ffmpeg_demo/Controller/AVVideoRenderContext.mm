//
//  AVVideoRenderContext.m
//  ffmpeg_demo
//
//  Created by yuxueqing on 2018/10/23.
//  Copyright © 2018年 yuxueqing. All rights reserved.
//

#import "AVVideoRenderContext.h"
#import <UIKit/UIKit.h>
#import <mutex>

namespace AVVideoPlayer {
    
AVDispatchQueue::AVDispatchQueue(const char *name, AVDispatchQueueType type)
    : m_name(name)
    , m_type(type) { }

AVDispatchQueue * AVDispatchQueue::AVMainQueue()
{
    static AVDispatchQueue *AVDiaptchMainQueue = nil;
    static std::once_flag onceFlag;
    std::call_once(onceFlag,[]()
    {
        AVDiaptchMainQueue = new AVDispatchQueue("AVDiaptchMainQueue",AVDispatchQueueType::AVDispatchMainQueue);
        AVDiaptchMainQueue->m_queue = dispatch_get_main_queue();
    });
    return AVDiaptchMainQueue;
}
    
AVDispatchQueue * AVDispatchQueue::AVDefaultQueue()
{
    static AVDispatchQueue *AVDiaptchDefaultQueue = nil;
    static std::once_flag onceFlag;
    std::call_once(onceFlag, []()
    {
        AVDiaptchDefaultQueue = new AVDispatchQueue("AVDiaptchDefaultQueue",AVDispatchQueueType::AVDispatchDefaultQueue);
        AVDiaptchDefaultQueue->m_queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0);
    });
    return AVDiaptchDefaultQueue;
}

AVDispatchQueue * AVDispatchQueue::AVLowQueue()
{
    static AVDispatchQueue *AVDiaptchLowQueue = nil;
    static std::once_flag onceFlag;
    std::call_once(onceFlag, []()
    {
        AVDiaptchLowQueue = new AVDispatchQueue("AVDiaptchLowQueue",AVDispatchQueueType::AVDispatchLowQueue);
        AVDiaptchLowQueue->m_queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_LOW, 0);
    });
    return AVDiaptchLowQueue;
}
    
AVDispatchQueue * AVDispatchQueue::AVHighQueue()
{
    static AVDispatchQueue *AVDiaptchHighQueue = nil;
    static std::once_flag onceFlag;
    std::call_once(onceFlag, []()
    {
        AVDiaptchHighQueue = new AVDispatchQueue("AVDiaptchHighQueue",AVDispatchQueueType::AVDispatchLowQueue);
        AVDiaptchHighQueue->m_queue = dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_HIGH, 0);
    });
    return AVDiaptchHighQueue;
}
    
void AVDispatchQueue::DispatchTaskAsync(AVDispatchQueueType type, dispatch_block_t block)
{
    DispatchTask(type, block, false);
}

void AVDispatchQueue::DispatchTaskSync(AVDispatchQueueType type, dispatch_block_t block)
{
    DispatchTask(type, block, true);
}

void AVDispatchQueue::DispatchTask(AVDispatchQueueType type, dispatch_block_t block, bool sync)
{
    dispatch_queue_t queue = _AVDispatchQueueFromType(type);
    DispatchTask(queue, block, sync);
}
    
dispatch_queue_t AVDispatchQueue::_AVDispatchQueueFromType(AVDispatchQueueType type)
{
    AVDispatchQueue *disptachQueue = _AVQueueFromType(type);
    return disptachQueue->m_queue;
}

void AVDispatchQueue::DispatchQueue(dispatch_block_t block, bool syncnous)
{
    DispatchTask(m_queue, block, syncnous);
}

void AVDispatchQueue::DispatchTask(dispatch_queue_t queue, dispatch_block_t block, bool sync)
{
    if (sync)
        dispatch_async(queue, block);
    else
        dispatch_sync(queue, block);
}
    
void AVDispatchQueue::DispatchTaskOnMainQueue(dispatch_block_t block, bool synchronous)
{
    AVDispatchQueue *mainQueue = AVMainQueue();
    mainQueue->DispatchTask(AVDispatchQueueType::AVDispatchMainQueue, block, synchronous);
}
    
void AVDispatchQueue::DispatchTaskOnGlobalQueue(AVDispatchQueueType type, dispatch_block_t block, bool synchronous)
{
    AVDispatchQueue *queue = AVDefaultQueue();
    queue->DispatchTask(AVDispatchQueueType::AVDispatchDefaultQueue, block, synchronous);
}

dispatch_queue_t AVDispatchQueue::Queue() const
{
    return m_queue;
}

const char* AVDispatchQueue::Name() const
{
    return m_name;
}

AVDispatchQueue::AVDispatchQueueType AVDispatchQueue::Type() const
{
    return m_type;
}

long long AVDispatchQueue::Priroty() const
{
    return m_priroty;
}

AVDispatchQueue* AVDispatchQueue::_AVQueueFromType(AVDispatchQueueType type)
{
    switch (type) {
        case AVDispatchQueueType::AVDispatchMainQueue:
            return AVMainQueue();
            break;
        case AVDispatchQueueType::AVDispatchLowQueue:
            return AVLowQueue();
            break;
        case AVDispatchQueueType::AVDispatchDefaultQueue:
            return AVDefaultQueue();
            break;
        case AVDispatchQueueType::AVDispatchHightQueue:
            return AVHighQueue();
            break;
        case AVDispatchQueueType::AVDispatchSerialQueue:
            return new AVDispatchQueue("AVDispatchQueueSerial",AVDispatchQueueType::AVDispatchSerialQueue);
            break;
        default:
            return nullptr;
            break;
    }
}
    
AVDispatchQueue::~AVDispatchQueue()
{
#if !OS_OBJECT_USE_OBJC
    dispatch_release(m_queue);
    m_queue = nullptr;
#endif
}
}

@implementation AVRenderDisplayLinkTimer
{
    CADisplayLink *m_displayLink;
    dispatch_block_t m_completion;
    AVVideoPlayer::AVDispatchQueue *m_dispatchQueue;
}

- (instancetype)initWithCompletion:(dispatch_block_t)completion
{
    if (self = [super init]) {
        m_completion = [completion copy];
        m_displayLink = [CADisplayLink displayLinkWithTarget:self
                                                    selector:@selector(fireDisplayLink:)];
        m_displayLink.paused = YES;
#if __IPHONE_OS_VERSION_MIN_REQUIRED >= __IPHONE_10_0
        m_displayLink.preferredFramesPerSecond = 30;
#else
        [m_displayLink setFrameInterval:2];
#endif
        m_dispatchQueue = AVVideoPlayer::AVDispatchQueue::AVMainQueue();
        m_dispatchQueue->DispatchTask(m_dispatchQueue->Queue(), ^{
            [self->m_displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSRunLoopCommonModes];
        }, false);
    }
    return self;
}

- (void)dealloc
{
    [self invalidate];
}

- (BOOL)isPause
{
    return m_displayLink.paused;
}

- (void)setIsPause:(BOOL)isPause
{
    m_displayLink.paused = isPause;
}

- (dispatch_block_t)block
{
    return m_completion;
}

- (void)setCompletionblock:(dispatch_block_t)block
{
    m_completion = [block copy];
}

- (void)invalidate
{
    [m_displayLink invalidate];
}

- (void)addRunLoopMode:(NSRunLoopMode)mode
{
    if (m_displayLink)
        [m_displayLink invalidate];
    else
        [m_displayLink addToRunLoop:[NSRunLoop mainRunLoop] forMode:mode];
}

- (void)fireDisplayLink:(CADisplayLink *)link
{
    if (m_completion)
        m_completion();
}

@end
