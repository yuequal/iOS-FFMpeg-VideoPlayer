//
//  AVDispatchQueue.h
//  AVVideoPlayer
//
//  Created by yuxueqing on 2018/9/15.
//  Copyright © 2018年 yuxueqing. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#define DISPATCH_QUEUE_MAIN_PRIORITY -4

@interface AVDispatchQueue : NSObject

/**
 * @brief 初始化生成实例方法
 * @param name 创建任务队列名字
 */
- (instancetype)initWithQueueName:(const char *)name;

- (instancetype)new NS_UNAVAILABLE;

/**
 * @return 获取主队列的类方法
 */
+ (AVDispatchQueue *)mainQueue;

/**
 * @return 获取全局默认优先级队列的类方法
 */
+ (AVDispatchQueue *)defaultQueue;

/**
 * @return 获取全局低优先级队列的类方法
 */
+ (AVDispatchQueue *)lowQueue;

/**
 * @return 获取全局高优先级队列的类方法
 */
+ (AVDispatchQueue *)highQueue;

/**
 * @return 当前运行线程是否是主线程
 */
+ (BOOL)isMainThreadRun;

/**
 * @return 返回任务队列
 */
- (dispatch_queue_t)queue;

/**
 * @return 对象中队列是否是主队列
 */
- (BOOL)isMainQueue;

/**
 * @return 当前线程正在执行的队列是否是当前对象队列
 */
- (BOOL)isCurrentQueue;

/**
 * @return 返回对象中队列优先级，主队列为-4
 */
- (int)queueType;

/**
 * @param  block 待入队列执行的任务，默认是异步方式
 */
- (void)dispatchTask:(dispatch_block_t)block;

/**
 * @param  block 待入队列执行的任务，默认是异步方式
 * @param  queue 执行任务的队列
 */
- (void)dispatchTask:(dispatch_block_t)block OnQueue:(dispatch_queue_t)queue;

/**
 * @param  block 待入队列执行的任务
 * @param  queue 执行任务的队列
 * @param  synchronous 是否同步
 
 *!!! 严禁在主线程中往主队列使用sync方式添加任务，会造成死锁,形如以下用法:
 *    SHManagerQueue *mainQueue = [SHManagerQueue mainQueue];
 *    [mainQueue dispatchTask:^{
 *    printf(" dead lock...");
 *    } OnQueue:mainQueue.queue synchronous:YES];
 */
- (void)dispatchTask:(dispatch_block_t)block OnQueue:(dispatch_queue_t)queue synchronous:(BOOL)synchronous;

/**
 * @brief 自定义队列中添加任务
 */
+ (void)dispatchTask:(dispatch_block_t)block OnQueue:(dispatch_queue_t)queue synchronous:(BOOL)synchronous;

/**
 * @brief 同步主队列执行
 */
+ (void)dispatchTaskSyncOnMainQueue:(dispatch_block_t)block;

/**
 * @brief 异步主队列执行
 */
+ (void)dispatchTaskAsyncOnMainQueue:(dispatch_block_t)block;

/**
 * @brief 确保UI操作在主线程
 */
+ (void)dispatchUITaskOnMainThread:(dispatch_block_t)block;

/**
 * @brief 任务在time秒后加入队列执行，默认主队列
 */
+ (void)disptachTaskAfterTime:(CGFloat)time task:(dispatch_block_t)block;

/**
 * @brief 任务在time秒后加入全局队列执行
 */
+ (void)disptachTaskOnGloablQueueAfterTime:(CGFloat)time task:(dispatch_block_t)block;

/**
 * @brief 任务在time秒后加入队列执行
 * @params time   多长时间后加入队列执行
 * @params block  要执行的任务
 * @params queue  执行任务的队列
 */
+ (void)disptachTaskAfterTime:(CGFloat)time task:(dispatch_block_t)block queue:(dispatch_queue_t)queue;

/**
 * @brief 动画事件
 * @params time 动画执行时长
 * @params block 动画过程执行的任务
 * @params completion 动画结束
 */
+ (void)disptachAnimationDuration:(CGFloat)time task:(dispatch_block_t)block completion:(void(^)(bool finished))completion;

@end

