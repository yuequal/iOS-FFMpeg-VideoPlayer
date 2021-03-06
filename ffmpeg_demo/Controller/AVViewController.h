//
//  AVViewController.h
//  ffmpeg_demo
//
//  Created by yuxueqing on 2018/9/9.
//  Copyright © 2018年 yuxueqing. All rights reserved.
//

#import <UIKit/UIKit.h>

NSString * const AVVideoPlayerDidBeginEditingNotification =         @"AVVideoPlayerDidBeginEditingNotification";
NSString * const AVVideoPlayerDidChangeNotification =               @"AVVideoPlayerDidChangeNotification";
NSString * const AVVideoPlayerWillResignNotification = @"AVVideoPlayerWillResignNotification";

@class AVViewController;

@protocol AVViewControllerDelegate <NSObject>

@optional

- (void)AVideoPlayerControllerTimeWillLoading:(AVViewController *)controller;

- (void)AVideoPlayerControllerTime:(AVViewController *)viewcontroller bitRate:(int)bitRate;

@end

@interface AVViewController : UIViewController

@property (nonatomic,readonly) CGFloat duration;

- (void)play;

- (void)restart;

- (void)prepareToPlay;

- (void)openUrl:(NSString *)url;

- (void)setPause:(BOOL)isPause;

- (void)seekTime:(double)time;

- (void)setDelegate:(id<AVViewControllerDelegate>)delegate;

@end

