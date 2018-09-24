//
//  AVVideoPlayerController.h
//  ffmpeg_demo
//
//  Created by yuxueqing on 2018/9/14.
//  Copyright © 2018年 yuxueqing. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <Foundation/Foundation.h>
#import "AVViewController.h"

@interface AVVideoPlayerController : NSObject
{
@private
    AVViewController *_ViewController;
}

@property (nonatomic, readonly) UIView *view;

- (instancetype)initWithContentURL:(NSString *)url;

@end
