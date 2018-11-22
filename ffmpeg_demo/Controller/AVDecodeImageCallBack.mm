//
//  AVDecodeImageCallBack.mm
//  ffmpeg_demo
//
//  Created by yuxueqing on 2018/10/25.
//  Copyright © 2018年 yuxueqing. All rights reserved.
//

#include "AVDecodeImageCallBack.h"

namespace AVVideoPlayer {
    
bool VideoDecode::PrefersLateDecoding() const
{
    return false;
}

const char* VideoDecode::DecodeName() const
{
    return "AVVideoDecoder";
}

int32_t VideoDecode::Release()
{
    return -1;
}
    
}
