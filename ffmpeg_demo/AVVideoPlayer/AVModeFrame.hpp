//
//  AVModeFrame.hpp
//  ffmpeg_demo
//
//  Created by yuxueqing on 2018/9/18.
//  Copyright © 2018年 yuxueqing. All rights reserved.
//

#ifndef AVModeFrame_hpp
#define AVModeFrame_hpp

#include <memory>
//#include <optional>
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavutil/avutil.h>
#include <libavformat/avformat.h>
}

namespace AVVideoPlayer {
    class AVModeFrame
    {
    public:
        double Pts() const;
        void SetPts(double pts);
        std::shared_ptr<AVFrame> frame() const;
        
        static AVModeFrame* createMFrame(AVFrame *frame);
        
    private:
        std::shared_ptr<AVFrame> m_avframe;
        double m_pts = 0.0;
        double m_duration = 0.0;
    };
}

#endif /* AVModeFrame_hpp */
