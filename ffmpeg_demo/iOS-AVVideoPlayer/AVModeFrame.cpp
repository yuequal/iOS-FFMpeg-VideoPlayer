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
    mframe->m_avframe = std::shared_ptr<AVFrame>(frame);
    mframe->SetPts(frame->pts);
    return mframe;
}

std::shared_ptr<AVFrame> AVModeFrame::frame() const
{
    return m_avframe;
}
    
void AVModeFrame::SetPts(double pts)
{
    m_pts = pts;
}

double AVModeFrame::Pts() const
{
    return m_pts;
}
    
int AVModeFrame::Width() const
{
    return m_width;
}
    
int AVModeFrame::Height() const
{
    return m_height;
}

uint32_t AVModeFrame::Size() const
{
    return m_size;
}

int64_t AVModeFrame::TimeStamp() const
{
    return m_timeStamp;
}
    
void AVModeFrame::SetTimeStamp(int64_t ts)
{
    m_timeStamp = ts;
}

    
}
