//
//  AVOptional.hpp
//  ffmpeg_demo
//
//  Created by yuxueqing on 2018/10/25.
//  Copyright © 2018年 yuxueqing. All rights reserved.
//

#ifndef AVOptional_hpp
#define AVOptional_hpp

#include <type_traits>

namespace AVVideoPlayer {
    
template<typename T>
class AVOptional
{
    
    using data_t = typename std::aligned_storage<sizeof(T), std::alignment_of<T>::value>::type;
    
public:
    
    AVOptional() : m_hasInit(false) {}
    AVOptional(const T& v)
    {
        Create(v);
    }
    
    AVOptional(T&& v) : m_hasInit(false)
    {
        Create(std::move(v));
    }
    
    ~AVOptional()
    {
        Destroy();
    }
    
    AVOptional(const AVOptional& other) : m_hasInit(false)
    {
        if (other.IsInit())
            Assign(other);
    }
    
    AVOptional(AVOptional&& other) : m_hasInit(false)
    {
        if (other.IsInit())
        {
            Assign(std::move(other));
            other.Destroy();
        }
    }
    
    AVOptional& operator=(AVOptional &&other)
    {
        Assign(std::move(other));
        return *this;
    }
    
    AVOptional& operator=(const AVOptional &other)
    {
        Assign(other);
        return *this;
    }
    
    template<class... Args>
    void emplace(Args&&... args)
    {
        Destroy();
        Create(std::forward<Args>(args)...);
    }
    
    bool IsInit() const { return m_hasInit; }
    
    explicit operator bool() const { return IsInit();
        
    }
    
    T& operator*()
    {
        return *((T*) (&m_data));
    }
    
    T const& operator*() const
    {
        if (IsInit())
        {
            return *((T*) (&m_data));
        }
    }
    
    bool operator == (const AVOptional<T>& rhs) const
    {
        return (!bool(*this)) != (!rhs) ? false : (!bool(*this) ? true : (*(*this)) == (*rhs));
    }
    
    bool operator < (const AVOptional<T>& rhs) const
    {
        return !rhs ? false : (!bool(*this) ? true : (*(*this) < (*rhs)));
    }
    
    bool operator != (const AVOptional<T>& rhs)
    {
        return !(*this == (rhs));
    }
    
private:
    
    template<class... Args>
    void Create(Args&&... args)
    {
        new (&m_data) T(std::forward<Args>(args)...);
        m_hasInit = true;
    }
    
    void Destroy()
    {
        if (m_hasInit)
        {
            m_hasInit = false;
            ((T*) (&m_data))->~T();
        }
    }
    
    void Assign(const AVOptional& other)
    {
        if (other.IsInit())
        {
            Copy(other.m_data);
            m_hasInit = true;
        }
        else
        {
            Destroy();
        }
    }
    
    void Assign(AVOptional&& other)
    {
        if (other.IsInit())
        {
            Move(std::move(other.m_data));
            m_hasInit = true;
            other.Destroy();
        }
        else
        {
            Destroy();
        }
    }
    
    void Move(data_t&& val)
    {
        Destroy();
        new (&m_data) T(std::move(*((T*)(&val))));
    }
    
    void Copy(const data_t& val)
    {
        Destroy();
        new (&m_data) T(*((T*) (&val)));
    }
    
private:
    bool m_hasInit;
    data_t m_data;
};
}

#endif /* AVOptional_hpp */
