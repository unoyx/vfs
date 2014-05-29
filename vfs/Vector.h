#ifndef _VECTOR_H_
#define _VECTOR_H_
#include <assert.h>

template<typename T>
class Vector
{
public:
    Vector(void);
    Vector(const Vector<T>& v);
    Vector<T>& operator=(const Vector<T>& v);
    Vector(Vector<T>&& v);
    Vector<T>& operator=(Vector<T>&& v);
    ~Vector(void);

    T& operator[](int n);
    const T& operator[](int n) const;

    /* 在pos之前插入新元素 */
    void insert(int pos, const T& e);
    void remove(int pos);
    void append(const T& e);
    void clear(void);
    // TODO
    // void copy(const T* p, int len);
    int size(void) const;
    bool isEmpty(void) const;
private:
    T* m_data;
    int m_size;
};

template<typename T>
Vector<T>::Vector(void)
    :m_data(nullptr),
    m_size(0)
{
}

template<typename T>
Vector<T>::Vector(const Vector<T>& v)
    :m_data(new T[v.m_size]),
    m_size(v.m_size)
{
    for (int i = 0; i < m_size; ++i)
    {
        m_data[i] = v[i];
    }
}

template<typename T>
Vector<T>::Vector(Vector<T>&& v)
    :m_data(v.m_data),
    m_size(v.m_size)
{
    v.m_data = nullptr;
    v.m_size = 0;
}

template<typename T>
Vector<T>& Vector<T>::operator=(Vector<T>&& v)
{
    m_data = v.m_data;
    m_size = v.m_size;
    v.m_data = 0;
    v.m_size = 0;
    return *this;
}

template<typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& v)
{
    if (v.m_data != m_data)
    {
        T* ndata = new T[v.m_size];
        assert(ndata);
        for (int i = 0; i < v.m_size; ++i)
        {
            ndata[i] = v.m_data[i];
        }
        if (m_data)
        {
            delete[] m_data;
            m_data = nullptr;
        }
        m_data = ndata;
        m_size = v.m_size;
    }
    return *this;
}

template<typename T>
Vector<T>::~Vector(void)
{
    if (m_data)
    {
        delete[] m_data;
        m_data = nullptr;
        m_size = 0;
    }
}

template<typename T>
T& Vector<T>::operator[](int n)
{
    assert(n >= 0 && n < m_size);
    return m_data[n];
}

template<typename T>
const T& Vector<T>::operator[](int n) const
{
    assert(n >= 0 && n < m_size);
    return m_data[n];
}

template<typename T>
void Vector<T>::insert(int pos, const T& e)
{
    assert(pos >= 0 && pos <= m_size);
    T* ndata = new T[m_size + 1];
    assert(ndata);
    int i = 0;
    for (; i < pos; ++i)
    {
        ndata[i] = m_data[i];
    }
    ndata[i] = e;
    for (; i < m_size; ++i)
    {
        ndata[i + 1] = m_data[i];
    }
    if (m_data)
    {
        delete[] m_data;
        m_data = nullptr;
    }
    m_data = ndata;
    ++m_size;
}

template<typename T>
void Vector<T>::remove(int pos)
{
    assert(pos >= 0 && pos < m_size);
    T* ndata = new T[m_size - 1];
    assert(ndata);
    int i = 0;
    for (; i < pos; ++i)
    {
        ndata[i] = m_data[i];
    }
    ++i;
    for (; i < m_size; ++i)
    {
        ndata[i - 1] = m_data[i];
    }
    if (m_data)
    {
        delete[] m_data;
        m_data = nullptr;
    }
    m_data = ndata;
    --m_size;
}

template<typename T>
void Vector<T>::append(const T& e)
{
    insert(m_size, e);
}

template<typename T>
void Vector<T>::clear(void)
{
    if (m_data)
    {
        delete[] m_data;
        m_data = nullptr;
        m_size = 0;
    }
}

template<typename T>
int Vector<T>::size(void) const
{
    return m_size;
}

template<typename T>
bool Vector<T>::isEmpty(void) const
{
    return m_size == 0;
}

#endif