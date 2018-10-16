//
//  AVReferencePtr.hpp
//  ffmpeg_demo
//
//  Created by yuxueqing on 2018/10/15.
//  Copyright © 2018年 yuxueqing. All rights reserved.
//

#ifndef AVReferencePtr_hpp
#define AVReferencePtr_hpp

#include <memory>

namespace AVVideoPlayer {
    
    template <class T>
    class AVRefPtr
    {
    public:
        AVRefPtr() : m_ptr(nullptr) {}
        AVRefPtr(T *p) : m_ptr(p)
        {
            if (m_ptr)
                
        }
        
        
    protected:
        T *m_ptr;
    };
}

#endif /* AVReferencePtr_hpp */
