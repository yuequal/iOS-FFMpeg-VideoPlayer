//
//  AVModeFrame.cpp
//  ffmpeg_demo
//
//  Created by yuxueqing on 2018/9/18.
//  Copyright © 2018年 yuxueqing. All rights reserved.
//

#include "AVModeFrame.hpp"

namespace AVVideoPlayer {
    
AVModeFrame* AVModeFrame::createMFrame(AVFrame *frame)
{
    AVModeFrame *mframe = new AVModeFrame();
  //  mframe->m_avframe = std::make_shared<AVFrame>(frame);
    return mframe;
}

std::shared_ptr<AVFrame> AVModeFrame::frame() const
{
    return m_avframe;
}
    
}
