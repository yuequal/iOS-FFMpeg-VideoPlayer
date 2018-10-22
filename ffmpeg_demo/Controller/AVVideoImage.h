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
    
template <typename T>
class AVFrameImageInterface
{
public:
    virtual ~AVFrameImageInterface() = default;
    virtual void Create(const T& t) = 0;
};

class AVVideoFrameImage : public AVFrameImageInterface <AVVideoImage>
{
public:
    AVVideoFrameImage(struct SwsContext *sctx,const AVCodecContext *ctx);
    
    virtual ~AVVideoFrameImage() { }
    
    virtual bool Open(const AVModeFrame* frame ,unsigned char* buffer,int width, int height);
    
    virtual void Create(const AVVideoImage& image);
    
    virtual UIImage *FrameImage(const unsigned char* data,int width, int height);
 
private:
    std::mutex m_mutex;
    
    struct SwsContext *m_swsContext;
    
    const AVCodecContext *m_codecContext;

};
}

@interface AVVideoImage : NSObject


@end
