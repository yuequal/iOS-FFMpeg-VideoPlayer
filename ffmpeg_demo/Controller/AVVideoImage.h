//
//  AVVideoImage.h
//  ffmpeg_demo
//
//  Created by yuxueqing on 2018/10/17.
//  Copyright © 2018年 yuxueqing. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#include <mutex>
#import "AVModeFrame.hpp"
extern "C"{
#include <libswscale/swscale.h>
}

@class AVVideoImage;

namespace AVVideoPlayer {
    
template <typename VideoImage,typename ModeFrame>
class AVFrameImageInterface
{
public:
    virtual ~AVFrameImageInterface() = default;
    
    virtual void Create(const VideoImage& image) = 0;

    virtual bool Open(const ModeFrame* frame,
                      unsigned char* buffer,
                      int width, int height) = 0;
    
    virtual VideoImage* FrameImage(const unsigned char* data,
                                   int width, int height) = 0;
};

class AVVideoFrameImage : public AVFrameImageInterface <AVVideoImage,AVModeFrame>
{
public:
    AVVideoFrameImage(struct SwsContext *sctx,const AVCodecContext *ctx);
    
    virtual ~AVVideoFrameImage() { }
    
    virtual bool Open(const AVModeFrame* frame ,unsigned char* buffer,int width, int height);
    
    virtual void Create(const AVVideoImage& image);
    
    virtual AVVideoImage *FrameImage(const unsigned char* data,int width, int height);
 
private:
    std::mutex m_mutex;
    
    struct SwsContext *m_swsContext;
    
    const AVCodecContext *m_codecContext;
};
}

@protocol AVVideoImageProtocol <NSObject>

- (int)width;
- (int)height;
- (UIImage *)image;

@end;

@interface AVVideoImage : NSObject <AVVideoImageProtocol>

@property (nonatomic, assign, readonly) int width;
@property (nonatomic, assign, readonly) int height;
@property (nonatomic, strong, readonly) UIImage *image;

+ (instancetype)videoImageWith:(UIImage *)image width:(int)width height:(int)height;

@end
