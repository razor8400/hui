#pragma once

namespace engine
{
    template<class T>
    class pointer
    {
    public:
        pointer();
        pointer(T* obj);
        pointer(std::nullptr_t ptr);
        pointer(const pointer<T>& other);
        ~pointer();
        
        pointer& operator=(T* obj);
        pointer& operator=(const pointer& other);
        
        bool operator==(const pointer& other) const;
        bool operator!=(const pointer& other) const;
        
        operator T*() const;
        
        T& operator*() const;
        T* operator->() const;
    private:
        T* m_obj = nullptr;
    };
}

