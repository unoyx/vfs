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
    void append(const T* e, int s);
    void extend(int c);
    void clear(void);
    // TODO
    // void copy(const T* p, int len);
    int size(void) const;
    bool isEmpty(void) const;
private:

    static const int INIT_CAPACITY;
    T* m_data;
    int m_size;
    int m_capacity;
};
template<typename T>
const int Vector<T>::INIT_CAPACITY = 8;

template<typename T>
Vector<T>::Vector(void)
    :m_data(nullptr),
    m_size(0),
    m_capacity(0)
{
//    extend(INIT_CAPACITY);
}

template<typename T>
Vector<T>::Vector(const Vector<T>& v)
    :m_data(new T[v.m_capacity]),
    m_size(v.m_size),
    m_capacity(v.m_capacity)
{
    for (int i = 0; i < m_size; ++i)
    {
        m_data[i] = v[i];
    }
}

template<typename T>
Vector<T>::Vector(Vector<T>&& v)
    :m_data(v.m_data),
    m_size(v.m_size),
    m_capacity(v.m_capacity)
{
    v.m_data = nullptr;
    v.m_size = 0;
    v.m_capacity = 0;
}

template<typename T>
Vector<T>& Vector<T>::operator=(Vector<T>&& v)
{
    if (m_data)
        delete m_data;
    m_data = v.m_data;
    m_size = v.m_size;
    m_capacity = v.m_capacity;
    v.m_data = 0;
    v.m_size = 0;
    v.m_capacity = 0;
    return *this;
}

template<typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& v)
{
    if (v.m_data != m_data)
    {
        T* ndata = new T[v.m_capacity];
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
        m_capacity = v.m_capacity;
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
        m_capacity = 0;
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
    if (m_size >= m_capacity)
    {
        extend(m_size + 1);
    }

    for (int i = m_size - 1; i >= pos; --i)
    {
        m_data[i + 1] = m_data[i];
    }
    m_data[pos] = e;
    ++m_size;
}

template<typename T>
void Vector<T>::remove(int pos)
{
    assert(pos >= 0 && pos < m_size);
    for (int i = pos + 1; i < m_size; ++i)
    {
        m_data[i - 1] = m_data[i];
    }
    --m_size;
}

template<typename T>
void Vector<T>::append(const T& e)
{
    insert(m_size, e);
}

template<typename T>
void Vector<T>::append(const T* e, int s)
{
    if (m_size + s > m_capacity)
    {
        extend(m_size + s);
    }
    for (int i = 0; i < s; ++i)
    {
        m_data[i + m_size] = e[i];
    }
    m_size += s;
}

template<typename T>
void Vector<T>::clear(void)
{
    m_size = 0;
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

template<typename T>
void Vector<T>::extend(int c)
{
    if (c <= m_capacity)
        return;
    if ((2 * m_capacity) < c)
    {
        m_capacity = c;
    } 
    else 
    {
        m_capacity *= 2;
    }
    T* ndata = new T[m_capacity];
    assert(ndata);

    for (int i = 0; i < m_size; ++i)
    {
        ndata[i] = m_data[i];
    }
    if (m_data)
    {
        delete[] m_data;
    }
    m_data = ndata;
}

#endif