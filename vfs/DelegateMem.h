#ifndef _DELEGATE_MEM_H_
#define _DELEGATE_MEM_H_

template<typename T>
class DelegateMem
{
public:
    DelegateMem(T* p);
    ~DelegateMem(void);

    T& operator[](int i);
    operator T*(void);

private:
    DelegateMem(void);
    DelegateMem(const DelegateMem& o);
    DelegateMem& operator=(const DelegateMem& o);

    T* m_data;
};

template<typename T>
DelegateMem<T>::DelegateMem(T* p)
    :m_data(p)
{

}

template<typename T>
DelegateMem<T>::~DelegateMem(void)
{
    if (m_data)
    {
        delete[] m_data;
        m_data = nullptr;
    }
}

template<typename T>
T& DelegateMem<T>::operator[](int i)
{
    return m_data[i];
}

template<typename T>
DelegateMem<T>::operator T*(void)
{
    return m_data;
}

#endif // _DELEGATE_MEM_H_

