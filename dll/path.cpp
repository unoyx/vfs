#include "path.h"
#include <assert.h>
#include "MyString.h"
#include "Vector.h"

static const int g_MAX_NAME_SIZE = 255;

// 在src中确定从位置pos开始，长度为len的范围内的首个与c匹配字符的位置
static int skipToEqual(const MyString& src, int pos, TCHAR c, int len = 0)
{
    assert(len >= 0);
    if (len == 0)
    {
        return src.find(c, pos);
    }
    for (int i = 0; i < len && (pos + i) < src.size(); ++i)
    {
        if (src[pos + i] == c)
        {
            return pos + i;
        }
    }
    return -1;
}

bool hasWildcard(const MyString& s)
{
    return (s.find('*') != -1) || (s.find('?') != -1);
}

bool match(const MyString& src, const MyString& pattern)
{
    int i = 0;
    int j = 0;
    for (; i < src.size() && j < pattern.size(); ++i, ++j)
    {
        if (pattern[j] == '*')
        {
            if ((j + 1) == pattern.size())
            {
                return true;
            }
            else
            {
                ++j;
//                int next_i = skipToEqual(src, i + 1, pattern[j]);
                int next_i = src.size() - 1;
                while (next_i > i && src[next_i] != pattern[j])
                {
                    --next_i;
                }
                if (next_i == i)
                {
                    return false;
                }
                i = next_i;
            }
        }
        else if (pattern[j] == '?')
        {
            if ((j + 1) == pattern.size())
            {
                if ((i + 1) == src.size())
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                ++j;
                int next_i = skipToEqual(src, i, pattern[j], 2);
                if (next_i == -1)
                {
                    return false;
                }
                i = next_i;
            }
        }
        else if (pattern[j] != src[i])
        {
            return false;
        }
    }
    if (i == src.size() && j == pattern.size())
    {
        return true;
    }
    // 说明src的长度短于pattern
    else if ((j + 1) == pattern.size())
    {
        if (pattern[j] == '*' || pattern[j] == '?')
        {
            return true;
        }
    }
    return false;
}


// test first
// \name
bool isVolumnRelative(MyString path)
{
    if (path.startWith(_T("\\")))
        return true;
    return false;
}

// test second 
// .
// ..
// .\
// ..\
// name
bool isRelative(MyString path)
{
    assert(!path.isEmpty());
    if (path.startWith(_T(".")))
    {
        return true;
    }
    else if (path.startWith(_T("\\")))
    {
        return true;
    }
    else
    {
        int pos = path.find(_T('\\'));
        MyString head = path.substr(0, pos);
        if (!head.isEmpty() && isLegalName(head))
            return true;
    }
    return false;
}

// C:\\
// test third
bool isAbs(MyString path)
{
    /*文件名需要忽略大小写 */
    if (path.startWith(_T("\\")))
    {
        return true;
    }
    else if (match(path, _T("?:*")) && _istalpha(path[0]))
    {
        return true;
    }
    return false;
}

bool isPath(MyString path)
{
    return isVolumnRelative(path) || isRelative(path) || isAbs(path);
}

bool isNormalizedPath(MyString path)
{
    if (isVolumnRelative(path) || isRelative(path))
    {
        return false;
    }
    else if (isAbs(path) && !path.endWith(_T("\\")))
    {
        return true;
    }
    return false;
}

bool isLegalChar(TCHAR c)
{
    return (c != _T('?')) 
        && (c != _T('"'))
        && (c != _T('/'))
        && (c != _T('\\'))
        && (c != _T('<'))
        && (c != _T('>'))
        && (c != _T('*'))
        && (c != _T('|'))
        && (c != _T(':'));
}

bool isLegalName(MyString name)
{
    for (int i = 0; i < name.size(); ++i)
    {
        if (!isLegalChar(name[i]))
            return false;
    }
    if (name.startWith(_T(" ")) || name.size() > g_MAX_NAME_SIZE)
        return false;
    return true;
}

MyString dirname(MyString path)
{
    assert(isPath(path));
    int pos = path.size() - 1;
    for (; pos >= 0 && path[pos] != _T('\\'); --pos)
    {
    }
    return path.substr(0, pos);
}

MyString basename(MyString path)
{
    assert(isPath(path));
    int pos = path.size() - 1;
    for (; pos >= 0 && path[pos] != _T('\\'); --pos)
    {
    }
    if (pos < 0)
    {
        return _T("");
    }
    return path.substr(pos + 1);
}

Vector<MyString> split(const MyString& path)
{
    Vector<MyString> ret;
    int spos = 0;
    while (spos < path.size())
    {
        int epos = path.find(_T('\\'), spos);
        if (epos != -1)
        {
            int i = epos + 1;
            while (i < path.size() && path[i] == _T('\\'))
                ++i;
            MyString name(path.substr(spos, epos - spos));
            ret.append(name);
            spos = i;
        }
        else if (spos < path.size())
        {
            MyString name(path.substr(spos));
            ret.append(name);
            spos = path.size();
        }
    }
    return ret;
}

MyString join(MyString path, MyString name)
{
    if (path[path.size() - 1] != _T('\\') 
        && !name.isEmpty() 
        && !name.startWith(_T("\\")))
    {
        path += _T("\\");
    }
    path += name;
    return path;
}
