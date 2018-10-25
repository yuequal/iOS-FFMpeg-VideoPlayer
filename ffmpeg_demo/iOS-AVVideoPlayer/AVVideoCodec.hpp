//
//  AVVideoCodec.hpp
//  ffmpeg_demo
//
//  Created by yuxueqing on 2018/10/25.
//  Copyright © 2018年 yuxueqing. All rights reserved.
//

#ifndef AVVideoCodec_hpp
#define AVVideoCodec_hpp

#include <string>

namespace AVVideoPlayer {
    
class AVVideoCodec
{
public:
    AVVideoCodec();
public:
    bool operator==(const AVVideoCodec& other) const = delete;
    bool operator!=(const AVVideoCodec& other) const = delete;
public:
    uint16_t m_width;
    uint16_t m_height;
    unsigned int m_bitRate;
    uint32_t m_frameRate;
    bool m_active;
    const uint8_t* m_data;
    const char* m_name;
};
}

#endif /* AVVideoCodec_hpp */
