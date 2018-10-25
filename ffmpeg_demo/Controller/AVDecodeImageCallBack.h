//
//  AVDecodeImageCallBack.h
//  ffmpeg_demo
//
//  Created by yuxueqing on 2018/10/25.
//  Copyright © 2018年 yuxueqing. All rights reserved.
//

#ifndef AVDecodeImageCallBack_h
#define AVDecodeImageCallBack_h

#include "AVOptional.hpp"
#include "AVModeFrame.hpp"
#include "AVVideoCodec.hpp"
#include "AVVideoImage.h"

namespace AVVideoPlayer {
    
class AVDecodeImageCallBack
{
public:
    virtual ~AVDecodeImageCallBack() {}
    
    virtual int32_t AVDecoded(AVModeFrame& decodeFrame) = 0;
    
    virtual int32_t AVDecoded(AVModeFrame& decodeFrame, AVOptional<int32_t> decodec_timssp);
    
    virtual void AVDecoded(AVModeFrame& decodeFrame, AVOptional<int32_t> decoded_times, AVOptional<int8_t> qp);
    
    virtual int32_t AVReceivedDecodedFrame(const uint16_t pictureID);
    
};
    
class VideoDecode
{
public:
    virtual ~VideoDecode() {}

    virtual int32_t InitVideoDecode(const AVVideoCodec *codec, int ns = 0) = 0;
    
    virtual int32_t Decode( const AVVideoFrameImage& image, const std::string& info, int64_t render_times) = 0;
    
    virtual int32_t RegisterCallBack(AVDecodeImageCallBack* callBack) = 0;
    
    virtual int32_t Release() = 0;
    
    virtual bool PrefersLateDecoding() const;
    
    virtual const char* DecodeName() const;
};
    
}

#endif /* AVDecodeImageCallBack_h */
