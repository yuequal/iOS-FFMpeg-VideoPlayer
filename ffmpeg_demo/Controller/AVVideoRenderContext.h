//
//  AVVideoRenderContext.h
//  ffmpeg_demo
//
//  Created by yuxueqing on 2018/10/23.
//  Copyright © 2018年 yuxueqing. All rights reserved.
//

#import <Foundation/Foundation.h>

@class AVRenderDisplayLinkTimer;

namespace AVVideoPlayer {
class AVVideoRenderContext
{
    
private:
    AVRenderDisplayLinkTimer *m_displayLinkTimer;
};
    
class AVDispatchQueue
{
    
public:
    enum class AVDispatchQueueType { AVDispatchMainQueue, AVDispatchLowQueue,AVDispatchDefaultQueue,AVDispatchHightQueue ,AVDispatchSerialQueue };
public:
    explicit AVDispatchQueue(const char *name,AVDispatchQueueType type);
    AVDispatchQueue(long long priroty);
    virtual ~AVDispatchQueue();
    
public:
    dispatch_queue_t Queue() const;
    const char* Name() const;
    AVDispatchQueueType Type() const;
    long long Priroty() const;
    
public:
    static AVDispatchQueue *AVDefaultQueue();
    static AVDispatchQueue *AVMainQueue();
    static AVDispatchQueue *AVLowQueue();
    static AVDispatchQueue *AVHighQueue();
    
public:
    virtual void DispatchQueue(dispatch_block_t block, bool syncnous);
    
public:
    void DispatchTaskSync(AVDispatchQueueType type, dispatch_block_t block);
    void DispatchTaskAsync(AVDispatchQueueType type , dispatch_block_t block);
    void DispatchTask(AVDispatchQueueType type,dispatch_block_t block, bool sync);
    void DispatchTask(dispatch_queue_t queue, dispatch_block_t block, bool sync);
    
public:
    static void DispatchTaskOnMainQueue(dispatch_block_t block, bool synchronous);
    
    static void DispatchTaskOnGlobalQueue(AVDispatchQueueType type, dispatch_block_t block, bool synchronous);
    
private:
    AVDispatchQueue* _AVQueueFromType(AVDispatchQueueType type);
    dispatch_queue_t _AVDispatchQueueFromType(AVDispatchQueueType type);
    
private:
    dispatch_queue_t m_queue;
    const char *m_name;
    long long m_priroty;
    AVDispatchQueueType m_type;
};
}

@interface AVRenderDisplayLinkTimer : NSObject

@property (nonatomic, assign) BOOL isPause;

@property (nonatomic, readonly) dispatch_block_t block;

- (instancetype)initWithCompletion:(dispatch_block_t)completion;

- (void)setCompletionblock:(dispatch_block_t)block;

- (void)addRunLoopMode:(NSRunLoopMode)mode;

- (void)invalidate;

@end
