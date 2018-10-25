//
//  AVVideoDecoder.h
//  ffmpeg_demo
//
//  Created by yuxueqing on 2018/10/25.
//  Copyright © 2018年 yuxueqing. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "AVDecodeImageCallBack.h"
#import "AVDecode.hpp"

namespace AVVideoPlayer {

class AVVideoDecoderImpl : public VideoDecode
{
public:
    static std::unique_ptr<AVVideoDecoderImpl> Create();
    static bool IsCodec();
public:
    ~AVVideoDecoderImpl() override {}
};

class AVVideoDecoder : public AVVideoDecoderImpl
{
public:
    AVVideoDecoder();
    ~AVVideoDecoder() override;
    
    int32_t InitVideoDecode(const AVVideoCodec *codec, int32_t ns) override;
    
    int32_t Release() override;
    
    int32_t RegisterCallBack(AVDecodeImageCallBack* callBack) override;
    
    int32_t Decode(const AVVideoFrameImage& image, const std::string& info, int64_t render_times = -1) override;
    
    const char* DecodeName() const override;
    
private:
    static int AVGetBuffer(AVCodecContext* context, AVModeFrame *frame, int flags);
    
    std::unique_ptr<AVCodecContext, AVCodecContextFree> m_codecContext;
    
    std::unique_ptr<AVModeFrame, AVModeFrameFree> m_modeFrame;
    
    AVDecodeImageCallBack *m_decodeCallBack;
};
    

}


@interface AVVideoDecoder : NSObject

@end
