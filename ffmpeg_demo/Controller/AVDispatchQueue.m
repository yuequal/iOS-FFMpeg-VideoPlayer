//
//  AVDispatchQueue.m
//  AVVideoPlayer
//
//  Created by yuxueqing on 2018/9/15.
//  Copyright © 2018年 yuxueqing. All rights reserved.
//

#import "AVDispatchQueue.h"

static inline bool
_is_execute_dispatch_queue_specific(const char *name,void *context)
{
    if (name == NULL || context == NULL) return NO;
    return dispatch_get_specific(name) == context;
}

static inline bool
_is_dispatch_queue_specific(const char *name,void *context,dispatch_queue_t queue)
{
    if (name == NULL || context == NULL || queue == NULL) return NO;
    const char *qctx = (const char *)dispatch_queue_get_specific(queue,name);
    return context == qctx;
}

static inline void
_set_dispatch_queue_specific(const char *name,void *context,dispatch_queue_t queue)
{
    if (name == NULL || queue == NULL) return;
    NSString *qname = [NSString stringWithUTF8String:name];
    context = (__bridge void *)[qname stringByAppendingString:@"Context"];
    dispatch_queue_set_specific(queue, (const void *)name, (void *)context, NULL);
}

static inline bool _is_main_thread_run()
{
    static const char *mainQueueKey = "com.QianFanLive.mainQueue";
    static void *mainQueueContext = @"com.QianFanLive.mainQueueContext";
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        _set_dispatch_queue_specific(mainQueueKey, mainQueueContext, dispatch_get_main_queue());
    });
    
    return _is_execute_dispatch_queue_specific(mainQueueKey, mainQueueContext);
}

static inline const char *
_dispatch_queue_name_from_priority(long priority)
{
    switch (priority) {
        case DISPATCH_QUEUE_MAIN_PRIORITY:              return
            "com.QianFanLive.main.queue";
        case DISPATCH_QUEUE_PRIORITY_LOW:               return "com.QianFanLive.concurrent.low.queue";
        case DISPATCH_QUEUE_PRIORITY_DEFAULT:           return "com.QianFanLive.concurrent.default.queue";
        case DISPATCH_QUEUE_PRIORITY_HIGH:              return "com.QianFanLive.concurrent.high.queue";
        default:                                        return
            "com.QianFanLive.default.queue";
    }
}

@interface AVDispatchQueue ()
{
    BOOL             m_isMainQueue; //是否主队列
    
    BOOL             m_isConcurrentQueue; //是否全局队列
    
    int              m_concurrentType; //队列优先级
    
    void*            m_context; //队列上下文
    
    const char*      m_name; //队列名称
    
    dispatch_queue_t m_queue;  //任务队列
    
}
@end
@implementation AVDispatchQueue

- (void)dealloc
{
#if !OS_OBJECT_USE_OBJC
    dispatch_release(self->m_queue);
    self->m_queue = nil;
#endif
    
}

- (instancetype)init
{
    return [self initWithQueueName:"com.QianFanLive.default.queue"];
}

- (instancetype)initWithQueueName:(const char *)name
{
    self = [super init];
    if (self) {
        self->m_name = name;
        self->m_queue = dispatch_queue_create(name, NULL);
        _set_dispatch_queue_specific(self->m_name, self->m_context, self->m_queue);
    }
    return self;
}

- (instancetype)initWithQueueType:(int)type
{
    self = [super init];
    if (self) {
        self->m_name = [[self class] getName:type];
        self->m_isMainQueue = NO;
        self->m_concurrentType = type;
        self->m_isConcurrentQueue = YES;
        self->m_queue = dispatch_get_global_queue(type, 0);
        _set_dispatch_queue_specific(self->m_name, self->m_context, self->m_queue);
    }
    return self;
}
+ (AVDispatchQueue *)mainQueue
{
    static AVDispatchQueue *queue = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^
   {
      queue = [[self class] alloc];
      queue->m_queue = dispatch_get_main_queue();
      queue->m_isMainQueue = true;
      queue->m_concurrentType = DISPATCH_QUEUE_MAIN_PRIORITY;
    });
    return queue;
}

+ (AVDispatchQueue *)defaultQueue
{
    static AVDispatchQueue *queue = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^
    {
        queue = [[self alloc] initWithQueueType:DISPATCH_QUEUE_PRIORITY_DEFAULT];
    });
    return queue;
}

+ (AVDispatchQueue *)lowQueue
{
    static AVDispatchQueue *queue = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^
    {
        queue = [[self alloc] initWithQueueType:DISPATCH_QUEUE_PRIORITY_LOW];
    });
    return queue;
}

+ (AVDispatchQueue *)highQueue
{
    static AVDispatchQueue *queue = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^
    {
        queue = [[self alloc] initWithQueueType:DISPATCH_QUEUE_PRIORITY_HIGH];
    });
    return queue;
}

+ (const char *)getName:(long)type
{
    return _dispatch_queue_name_from_priority(type);
}

+ (BOOL)isMainThreadRun
{
    return [NSThread isMainThread];
}

- (const char *)name
{
    return self->m_name;
}

- (dispatch_queue_t)queue
{
    return self->m_queue;
}

- (BOOL)isMainQueue
{
    return self->m_isMainQueue;
}

- (BOOL)isConcurrentQueue
{
    return self->m_isConcurrentQueue;
}

- (BOOL)isCurrentQueue
{
    if (self->m_queue == nil || self->m_name == nil) {
        return false;
    }
    
    if (self->m_isMainQueue)
        return [[self class] isMainThreadRun];
    else
        return _is_execute_dispatch_queue_specific(self->m_name,self->m_context);
}

- (int)concurrentQueueType
{
    return self->m_concurrentType;
}

- (void)dispatchTask:(dispatch_block_t)block
{
    [self dispatchTask:[block copy] OnQueue:self->m_queue];
}

- (void)dispatchTask:(dispatch_block_t)block OnQueue:(dispatch_queue_t)queue
{
    [self dispatchTask:[block copy] OnQueue:queue synchronous:false];
}

- (void)_dispatchTask:(dispatch_block_t)block onQueue:(dispatch_queue_t)queue synchronous:(BOOL)synchronous
{
    if (synchronous)
        dispatch_sync(queue, block);
    else
        dispatch_async(queue, block);
}

- (void)dispatchTask:(dispatch_block_t)block OnQueue:(dispatch_queue_t)queue synchronous:(BOOL)synchronous
{
    if (block == nil || queue == nil) return;
    
    if (_is_dispatch_queue_specific(self->m_name,self->m_context, queue)) {
        if (self->m_isMainQueue) {
            if ([[self class] isMainThreadRun])
                block();
            else
                [self _dispatchTask:block onQueue:self->m_queue synchronous:synchronous];
        } else {
            if (_is_execute_dispatch_queue_specific(self->m_name,self->m_context)) //m_queue
                block();
            else
                [self _dispatchTask:block onQueue:self->m_queue synchronous:synchronous];
        }
    } else {
        [self _dispatchTask:block onQueue:queue synchronous:synchronous];
    }
}

+ (void)dispatchTask:(dispatch_block_t)block OnQueue:(dispatch_queue_t)queue synchronous:(BOOL)synchronous
{
    [[self class] dispatchTask:block OnQueue:queue synchronous:synchronous];
}

+ (void)dispatchTaskSyncOnMainQueue:(dispatch_block_t)block
{
    [[[self class] mainQueue] dispatchTask:block OnQueue:[[[self class] mainQueue] queue] synchronous:YES];
}

+ (void)dispatchTaskAsyncOnMainQueue:(dispatch_block_t)block
{
    [[[self class] mainQueue] dispatchTask:block OnQueue:[[[self class] mainQueue] queue] synchronous:NO];
}

+ (void)dispatchUITaskOnMainThread:(dispatch_block_t)block
{
    if ([[self class] isMainThreadRun]) {
        if (block) block();
    } else {
        [[[self class] mainQueue] dispatchTask:block OnQueue:[[[self class] mainQueue] queue] synchronous:YES];
    }
}

+ (void)disptachTaskAfterTime:(CGFloat)time task:(dispatch_block_t)block
{
    [[self class] disptachTaskAfterTime:time task:block queue:[[[self class] mainQueue] queue]];
}

+ (void)disptachTaskOnGloablQueueAfterTime:(CGFloat)time task:(dispatch_block_t)block
{
    [[self class] disptachTaskAfterTime:time task:block queue:[[[self class] defaultQueue] queue]];
}

+ (void)disptachTaskAfterTime:(CGFloat)time task:(dispatch_block_t)block queue:(dispatch_queue_t)queue
{
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(time * NSEC_PER_SEC)), queue, block);
}

+ (void)disptachAnimationDuration:(CGFloat)time task:(dispatch_block_t)block completion:(void (^)(bool finished))completion
{
    [[self class] dispatchUITaskOnMainThread:^
     {
         [UIView animateWithDuration:time animations:block completion:^(BOOL finished) {
             if (completion)
                 completion(finished);
         }];
     }];
}

@end

