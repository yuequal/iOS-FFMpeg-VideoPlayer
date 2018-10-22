//
//  AVVideoImage.m
//  ffmpeg_demo
//
//  Created by yuxueqing on 2018/10/17.
//  Copyright © 2018年 yuxueqing. All rights reserved.
//

#import "AVVideoImage.h"

namespace AVVideoPlayer {
    
AVVideoFrameImage::AVVideoFrameImage(struct SwsContext *sctx, const AVCodecContext* ctx)
    : m_swsContext(sctx)
    , m_codecContext(ctx) {}
    
bool AVVideoFrameImage::Open(const AVModeFrame* modeFrame ,unsigned char *buffer, int width, int height)
{
    if (!m_codecContext || !modeFrame) return false;
    std::lock_guard<std::mutex> lock(m_mutex);
    m_swsContext = sws_getCachedContext(m_swsContext, m_codecContext->width,
                                   m_codecContext->height,
                                   m_codecContext->pix_fmt,
                                   width, height,
                                   AV_PIX_FMT_RGB24,
                                   SWS_BICUBIC, NULL, NULL, NULL);
    if (!m_swsContext) return false;
    
    uint8_t * data[AV_NUM_DATA_POINTERS]={0};
    data[0] = (uint8_t*)buffer;
    int linesize[AV_NUM_DATA_POINTERS] = {0};
    linesize[0] = width * 4;
    
    std::shared_ptr<AVFrame> frame = modeFrame->Frame();
    sws_scale(m_swsContext, (const uint8_t* const*)frame->data, frame->linesize, 0, m_codecContext->height,data,linesize);
    return true;
}

UIImage * AVVideoFrameImage::FrameImage(const unsigned char* data,int width,int height)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    int linesize[AV_NUM_DATA_POINTERS] = {0};
    linesize[0] = width * 4;
    CGBitmapInfo bitmapInfo = kCGBitmapByteOrderDefault;
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    void * colorData = NULL;
    memcpy(&colorData, &data, sizeof(data));
    CGDataProviderRef provider = CGDataProviderCreateWithData(NULL, colorData, sizeof(colorData), NULL);
    CGImageRef cgImage = CGImageCreate(width,height,8,24,linesize[0],colorSpace,bitmapInfo,provider,NULL,NO,kCGRenderingIntentDefault);
    UIImage * image = [UIImage imageWithCGImage:cgImage];
    CGImageRelease(cgImage);
    CGColorSpaceRelease(colorSpace);
    CGDataProviderRelease(provider);
    return image;
}
    
void AVVideoFrameImage::Create(const AVVideoImage& image)
{}
    
}


@implementation AVVideoImage

@end
