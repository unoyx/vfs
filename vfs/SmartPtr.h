#ifndef _SMART_PTR_
#define _SMART_PTR_

template<typename T>
class SmartPtr
{
public:
    SmartPtr(T* pt)
        :m_data(pt)
    {}

    ~SmartPtr()
    {
        delete m_data;
        m_data = nullptr;
    }

    SmartPtr(SmartPtr&& sp)
        :m_data(sp.m_data)
    {
        sp.m_data = nullptr;
    }

    SmartPtr& operator=(SmartPtr&& sp)
    {
        m_data = sp.m_data;
        sp.m_data = nullptr;
        return *this;
    }

    T& operator*()
    {
        return *m_data;
    }

    T* operator->()
    {
        return m_data;
    }

    bool isValid(void)
    {
        return m_data != nullptr;
    }

    T* get_data(void)
    {
        return m_data;
    }
private:
    SmartPtr(const SmartPtr&);
    SmartPtr& operator=(const SmartPtr&);
private:
    T* m_data;
};

#endif