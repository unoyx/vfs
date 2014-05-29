#include "CommandDel.h"
#include <stdio.h>
#include "state.h"
#include "path.h"
#include "DirHandler.h"
#include "DirIterator.h"
#include "VirtualDiskNode.h"
#include "CommandException.h"

CommandDel::CommandDel(void)
    :m_recursion(false)
{
}

CommandDel::~CommandDel(void)
{
}

void CommandDel::setPathes(const Vector<MyString>& pathes)
{
    if (pathes.size() == 0)
    {
        throw CommandException(_T("命令语法不正确\n"));
    }
    m_pathes = pathes;
    return;
}

void CommandDel::setSwitches(const Vector<MyString>& switches)
{
    for (int i = 0; i < switches.size(); ++i)
    {
        if (switches[i] == _T("/s"))
        {
            m_recursion = true;
        }
        else
        {
            throw CommandException(_T("无效的参数") + switches[i] + _T("\n"));
        }
    }
    return;
}

void CommandDel::exec(VirtualDiskNode* vfs)
{
    for (int i = 0; i < m_pathes.size(); ++i)
    {
        MyString path = m_pathes[i];
        if (!isNormalizedPath(path))
        {
            path = vfs->pathNormalize(path);
        }

        del(path, vfs);
    }
    return;
}

void CommandDel::del(MyString path, VirtualDiskNode* vfs)
{
    if (vfs->isFile(path))
    {
        vfs->deleteFile(path);
    }
    else if (hasWildcard(path))
    {
        MyString dir = dirname(path);
        MyString wirdcard = basename(path);
        DirHandler dirhandle = vfs->openDir(dir);
        for (DirIterator iter = dirhandle.getIterator(); !iter.isDone(); iter.next())
        {
            state s = iter.getItem();
            if (s.type == FILE_TYPE && match(s.name, wirdcard))
            {
                vfs->deleteFile(s.path);
            }
        }
    }
    else if (vfs->isDir(path))
    {
        _tprintf(_T("删除%s\\*,是否确认<Y/N>?"), path.c_str());
        TCHAR confirm = 0;
        _tscanf_s(_T("%c%*c"), &confirm, sizeof(confirm));
        if (confirm != 'y' && confirm != 'Y')
        {
            return;
        }

        DirHandler dir = vfs->openDir(path);
        for (DirIterator iter = dir.getIterator(); !iter.isDone(); iter.next())
        {
            state s = iter.getItem();
            if (s.type == FILE_TYPE)
            {
                vfs->deleteFile(s.path);
            }
            else if (s.type == DIR_TYPE && m_recursion && !s.name.startWith(_T(".")))
            {
                del(s.path, vfs);
            }
        }
    }
    else
    {
        throw CommandException(_T("系统找不到指定的目录\n"));
    }
    return;
}