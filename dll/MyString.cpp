#include "MyString.h"
#include <assert.h>
#include <string.h>
#include <ctype.h>

MyString::MyString(void)
    :m_data(new TCHAR[1]),
    m_size(0)
{
    m_data[0] = _T('\0');
}

MyString::MyString(const TCHAR* s)
    :m_data(new TCHAR[_tcslen(s) + 1]),
    m_size(static_cast<int>(_tcslen(s)))
{
    memcpy(m_data, s, (m_size + 1) * sizeof(TCHAR));
}

MyString::MyString(const TCHAR* s, int n)
    :m_data(new TCHAR[n + 1]),
    m_size(n)
{
    memcpy(m_data, s, m_size * sizeof(TCHAR));
    m_data[m_size] = _T('\0');
}

MyString::MyString(const MyString& s)
    :m_data(new TCHAR[s.m_size + 1]),
    m_size(s.m_size)

{
    memcpy(m_data, s.m_data, (m_size + 1) * sizeof(TCHAR));
}

MyString& MyString::operator=(const MyString& s)
{
    if (s.m_data != m_data)
    {
        TCHAR* ndata = new TCHAR[s.m_size + 1];
        assert(ndata);
        delete[] m_data;
        m_data = ndata;
        memcpy(m_data, s.m_data, (s.m_size + 1) * sizeof(TCHAR));
        m_size = s.m_size;
    }
    return *this;
}

MyString::MyString(MyString&& s)
    :m_data(s.m_data),
    m_size(s.m_size)
{
    s.m_data = nullptr;
    s.m_size = 0;
}

MyString& MyString::operator=(MyString&& s)
{
    if (m_data)
    {
        delete m_data;
    }
    m_data = s.m_data;
    m_size = s.m_size;
    s.m_data = nullptr;
    s.m_size = 0;
    return *this;
}

MyString::~MyString(void)
{
    delete[] m_data;
}

int MyString::size(void) const
{
    return m_size;
}

bool MyString::isEmpty(void) const
{
    return m_size == 0;
}

const TCHAR& MyString::operator[](int pos) const
{
    return const_cast<MyString*>(this)->operator[](pos);
}

TCHAR& MyString::operator[](int pos)
{
    assert(pos >= 0 && pos < m_size);
    return m_data[pos];
}

MyString& MyString::operator+=(const MyString& s)
{
    TCHAR* ndata = new TCHAR[m_size + s.m_size + 1];
    assert(ndata);
    memcpy(ndata, m_data, m_size * sizeof(TCHAR));
    memcpy(ndata + m_size, s.m_data, s.m_size * sizeof(TCHAR));
    ndata[m_size + s.m_size] = _T('\0');
    delete[] m_data;
    m_data = ndata;
    m_size += s.m_size;
    return *this;
}

bool MyString::operator==(const MyString& s) const
{
    if (m_data == s.m_data) 
    {
        return true;
    } 
    else if (m_size == s.m_size) 
    {
        if (memcmp(m_data, s.m_data, m_size * sizeof(TCHAR)) == 0)
            return true;
    }
    return false;
}

bool MyString::operator!=(const MyString& s) const
{
    return !operator==(s);
}

MyString MyString::substr(int pos, int len) const
{
    assert(pos >= 0);
    if (len == -1)
    {
        len = m_size - pos;
    }
    assert(len >= 0);

   return MyString(m_data + pos, len);
}

int MyString::find(const TCHAR c, int pos) const
{
    assert(pos >= 0);
    for (; pos < m_size; ++pos)
    {
        if (c == m_data[pos])
        {
            return pos;
        }
    }
    return -1;
}

int MyString::find(const TCHAR* s, int pos) const
{
    assert(pos >= 0);
    int len = _tcslen(s);
    for (; pos < m_size - len; ++pos)
    {
        if (memcmp(m_data + pos, s, len * sizeof(TCHAR)) == 0)
        {
            return pos;
        }
    }
    return -1;
}

int MyString::find(const MyString& s, int pos) const
{
    assert(pos >= 0 && pos < m_size);
    for (; pos < (m_size - s.m_size); ++pos)
    {
        if (memcmp(m_data + pos, s.m_data, s.m_size * sizeof(TCHAR)) == 0)
        {
            return pos;
        }
    }
    return -1;
}

bool MyString::startWith(const MyString& s) const
{
    return substr(0, s.m_size) == s;
}

bool MyString::startWith(const TCHAR* s) const
{
    return substr(0, _tcslen(s)) == s;
}

bool MyString::endWith(const MyString& s) const
{
    return substr(m_size - s.m_size) == s;
}

bool MyString::endWith(const TCHAR* s) const
{
    return substr(m_size - _tcslen(s)) == s;
}

MyString MyString::term(void)
{
    int pos_s = 0;
    for (; (pos_s < m_size) && _istspace(m_data[pos_s]); ++pos_s) 
    {}

    int pos_e = m_size;
    for (; (pos_e > 0) && (pos_e > pos_s) && _istspace(m_data[pos_e]); --pos_e)
    {}

    MyString ret(m_data + pos_s, pos_e - pos_s);
    return ret;
}

MyString MyString::toLower(void)
{
    MyString ret(*this);
    for (int i = 0; i < ret.size(); ++i)
    {
        if (isupper(ret[i]))
        {
            ret[i] = _tolower(ret[i]);
        }
    }
    return ret;
}

const TCHAR* MyString::c_str(void)
{
    return m_data;
}

const TCHAR* MyString::c_str(void) const
{
    return m_data;
}

MyString operator+(const MyString& lhs, const MyString& rhs)
{
    MyString temp;
    TCHAR* ndata = new TCHAR[lhs.m_size + rhs.m_size + 1];
    assert(ndata);
    memcpy(ndata, lhs.m_data, lhs.m_size * sizeof(TCHAR));
    memcpy(ndata + lhs.m_size, rhs.m_data, rhs.m_size * sizeof(TCHAR));
    ndata[lhs.m_size + rhs.m_size] = _T('\0');
    delete[] temp.m_data;
    temp.m_data = ndata;
    temp.m_size = lhs.m_size + rhs.m_size;
    return temp;
}

bool operator<(const MyString& lhs, const MyString& rhs)
{
    int i = 0;
    for (; i < lhs.m_size && i < rhs.m_size; ++i)
    {
        if (lhs[i] < rhs[i])
            return true;
        else if (lhs[i] > rhs[i])
            return false;
    }
    if (i < rhs.m_size)
        return true;
    else 
        return false;
}

bool operator>=(const MyString& lhs, const MyString& rhs)
{
    return !operator<(lhs, rhs);
}

bool operator>(const MyString& lhs, const MyString& rhs)
{
    int i = 0;
    for (; i < lhs.m_size && i < rhs.m_size; ++i)
    {
        if (lhs[i] > rhs[i])
            return true;
        else if (lhs[i] < rhs[i])
            return false;
    }
    if (i > rhs.m_size)
        return true;
    else 
        return false;
}

bool operator<=(const MyString& lhs, const MyString& rhs)
{
    return !operator>(lhs, rhs);
}