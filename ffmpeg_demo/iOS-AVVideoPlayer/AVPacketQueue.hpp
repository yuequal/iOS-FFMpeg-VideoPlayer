//
//  AVPacketQueue.hpp
//  ffmpeg_demo
//
//  Created by yuxueqing on 2018/9/24.
//  Copyright © 2018年 yuxueqing. All rights reserved.
//

#ifndef AVPacketQueue_hpp
#define AVPacketQueue_hpp

#include <mutex>
#include <list>
extern "C"{
#include <libavcodec/avcodec.h>
}

namespace AVVideoPlayer {
    template <typename T>
    class AVPacketQueue
    {
    public:
        AVPacketQueue(const AVPacketQueue&) = delete;
        AVPacketQueue& operator=(const AVPacketQueue &) = delete;
        
    public:
        AVPacketQueue(unsigned int maxSize = 10); //默认队列最大帧包为10个
        ~AVPacketQueue();
        
    public:
        void SetSize(unsigned int size);
        bool Push(T *t);
        bool Empty() const;
        T *Pop();
        
    private:
        std::mutex m_mutex;
        std::list<T *> m_buffer;
        unsigned int m_bufferSize = 0;
        std::condition_variable m_condition;
    };
    
    template <typename T>
    AVPacketQueue<T>::AVPacketQueue(unsigned int maxSize)
    {
        this->m_bufferSize = maxSize;
    }
    
    template <typename T>
    AVPacketQueue<T>::~AVPacketQueue()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_condition.wait(lock, [this]{ return this->Empty(); });
        while (!m_buffer.empty()) {
            AVPacket *packet = m_buffer.front();
            av_packet_unref(packet);
            av_packet_free(&packet);
            m_buffer.pop_front();
        }
    }
    
    template <typename T>
    T * AVPacketQueue<T>::Pop()
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_condition.wait(lock, [this]{ return this->Empty(); });
        if (m_buffer.empty()) {
            T *packet(nullptr);
            return packet;
        }
        T *packet = m_buffer.front();
        m_buffer.pop_front();
        return packet;
    }
    
    template <typename T>
    bool AVPacketQueue<T>::Push(T *t)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        if (m_buffer.size() > m_bufferSize) {
            return false;
        }
        m_buffer.push_back(t);
        lock.unlock();
        m_condition.notify_one();
        return true;
    }
    
    template <typename T>
    bool AVPacketQueue<T>::Empty() const
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        return m_buffer.empty();
    }
}

#endif /* AVPacketQueue_hpp */
