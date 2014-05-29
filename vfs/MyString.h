#ifndef _MY_STRING_H_
#define _MY_STRING_H_
#include <tchar.h>

class MyString
{
public:
    MyString(void);
    MyString(const TCHAR* s);
    MyString(const TCHAR* s, int n);
    MyString(const MyString& s);
    MyString& operator=(const MyString& s);
    MyString(MyString&& s);
    MyString& operator=(MyString&& s);
    ~MyString(void);

    int size(void) const;
    bool isEmpty(void) const;

    const TCHAR& operator[](int pos) const;
    TCHAR& operator[](int pos);

    MyString& operator+=(const MyString& s);
    bool operator==(const MyString& s) const;
    bool operator!=(const MyString& s) const;

    MyString substr(int pos = 0, int len = -1) const;
    int find(const TCHAR c, int pos = 0) const;
    int find(const TCHAR* s, int pos = 0) const;
    int find(const MyString& s, int pos = 0) const;
    bool startWith(const MyString& s) const;
    bool startWith(const TCHAR* s) const;
    bool endWith(const MyString& s) const;
    bool endWith(const TCHAR* s) const;

    /* 返回一个前后没有空白字符的新字符串 */
    MyString term(void);
    MyString toLower(void);
    const TCHAR* c_str(void);
    const TCHAR* c_str(void) const;

    friend MyString operator+(const MyString& lhs, const MyString& rhs);
    friend bool operator<(const MyString& lhs, const MyString& rhs);
    friend bool operator>=(const MyString& lhs, const MyString& rhs);
    friend bool operator>(const MyString& lhs, const MyString& rhs);
    friend bool operator<=(const MyString& lhs, const MyString& rhs);
private:
    TCHAR *m_data;
    int m_size;
};

MyString operator+(const MyString& lhs, const MyString& rhs);
bool operator<(const MyString& lhs, const MyString& rhs);
bool operator>=(const MyString& lhs, const MyString& rhs);
bool operator>(const MyString& lhs, const MyString& rhs);
bool operator<=(const MyString& lhs, const MyString& rhs);

#endif