#include "pointer.h"
//#include "engine.h"

#define REF() \
    if (m_obj) \
        m_obj->retain();

#define UNREF() \
    if (m_obj) \
        m_obj->release();

namespace engine
{
    template<class T>
    pointer<T>::pointer() : m_obj(nullptr)
    {
        
    }
    
    template<class T>
    pointer<T>::pointer(T* obj) : m_obj(obj)
    {
        REF()
    }
    
    template<class T>
    pointer<T>::pointer(std::nullptr_t ptr)
    {
        UNREF()
        
        m_obj = ptr;
    }
    
    template<class T>
    pointer<T>::pointer(const pointer<T>& other) : m_obj(other.m_obj)
    {
        REF()
    }
    
    template<class T>
    pointer<T>::~pointer()
    {
        UNREF()
    }
    
    template<class T>
    pointer<T>& pointer<T>::operator=(T* obj)
    {
        UNREF()
        
        m_obj = obj;
        
        REF()
        
        return *this;
    }
    
    template<class T>
    pointer<T>& pointer<T>::operator=(const pointer<T>& other)
    {
        UNREF()
        
        m_obj = other.m_obj;
        
        REF()
        
        return *this;
    }
    
    template<class T>
    bool pointer<T>::operator==(const pointer& other) const
    {
        return m_obj == other.m_obj;
    }
    
    template<class T>
    bool pointer<T>::operator!=(const pointer& other) const
    {
        return m_obj != other.m_obj;
    }
    
    template<class T>
    pointer<T>::operator T*() const
    {
        return m_obj;
    }
    
    template<class T>
    T& pointer<T>::operator*() const
    {
        return *m_obj;
    }
    
    template<class T>
    T* pointer<T>::operator->() const
    {
        return m_obj;
    }
}
