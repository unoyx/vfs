#include "CommandParser.h"
#include <ctype.h>
#include <stdio.h>
#include "path.h"
#include "MyString.h"
#include "Vector.h"

static int getPath(const MyString& cmd, int pos, MyString* p)
{
    int i = pos;
    while (i < cmd.size() && _istspace(cmd[i]))
        ++i;
    int j = i;
    if (cmd[j] == _T('"'))
    {
        ++j;
        while (j < cmd.size() && cmd[j] != _T('"'))
            ++j;
        if (j == cmd.size())
        {
            return -1;
        }
        else
        {
            *p = cmd.substr(i + 1, j - i - 1);
            return j + 1;
        }
    }
    else
    {
        while (j < cmd.size() && !_istspace(cmd[j]) && cmd[j] != _T('/'))
            ++j;
        *p = cmd.substr(i, j - i);
        return j;
    }
}

static int getSwitch(const MyString& cmd, int pos, MyString* s)
{
    int i = pos;
    while (i < cmd.size() && _istspace(cmd[i]))
        ++i;
    // 仅有首个字符为"\"
    int j = i + 1;
    while (j < cmd.size() && _istalpha(cmd[j]))
        ++j;
    *s = cmd.substr(i, j - i);
    return j;
}

// 取得命令名
static int getCmd(const MyString& cmd, int pos, MyString* name)
{
    int i = pos;
    while (i < cmd.size() && _istspace(cmd[i]))
        ++i;

    int j = i;
    while (j < cmd.size() && _istalpha(cmd[j]))
        ++j;

    *name = cmd.substr(i, j - i);
    return j;
}

int parse(const MyString& cmd, MyString* name, Vector<MyString>* pathes, Vector<MyString>* switches)
{
    if (pathes == nullptr || switches == nullptr)
    {
        return -1;
    }

    pathes->clear();
    switches->clear();

    int i = getCmd(cmd, 0, name);
    if (i == cmd.size())
    {
        return 0;
    }

    for (; i < cmd.size();)
    {
        if (_istspace(cmd[i]))
        {
            ++i;
        }
        else if (cmd[i] != _T('/'))
        {
            MyString path;
            i = getPath(cmd, i, &path);
            if (!isPath(path))
            {
                printf("文件名、目录名或卷标语法不正确\n");
                *name = _T("");
                pathes->clear();
                switches->clear();
                return -1;
            }
            pathes->append(path);
        }
        else if (cmd[i] == _T('/'))
        {
            MyString s;
            i = getSwitch(cmd, i, &s);
            switches->append(s);
        }
        else
        {
            assert(0);
            return -1;
        }
    }
    return 0;
}

