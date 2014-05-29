#include "CommandMkdir.h"
#include <stdio.h>
#include "path.h"
#include "DirHandler.h"
#include "VirtualFileSystem.h"
#include "CommandException.h"

CommandMkdir::CommandMkdir(void)
{
}

CommandMkdir::~CommandMkdir(void)
{
}

void CommandMkdir::setPathes(const Vector<MyString>& pathes)
{
    if (pathes.size() == 0)
    {
        throw CommandException(_T("命令语法不正确\n"));
    }
    m_pathes = pathes;
    return;
}

void CommandMkdir::setSwitches(const Vector<MyString>& switches)
{
    if (switches.size() > 0)
    {
        throw CommandException(_T("命令语法不正确\n"));
    }
    return;
}

void CommandMkdir::exec(VirtualDiskNode* vfs)
{
    for (int i = 0; i < m_pathes.size(); ++i)
    {
        MyString path = m_pathes[i];
        if (!isNormalizedPath(path))
        {
            path = vfs->pathNormalize(path);
        }

        if (!vfs->isExist(path))
        {
            if (mkdir(path, vfs) != 0)
                return;
        }
        else
        {
            throw CommandException(_T("子目录或文件") + path + _T("已经存在\n"));
        }
    }
    return;
}

int CommandMkdir::mkdir(MyString path, VirtualDiskNode* vfs)
{
    Vector<MyString> path_name = split(path);
    MyString p = path_name[0];
    //if (vfs->isExist(p) && vfs->isFile(p))
    //{
    //    printf("子目录或文件 %s 已经存在\n", p.c_str());
    //    return;
    //}
    for (int i = 1; i < path_name.size(); ++i)
    {
        if (!isLegalName(path_name[i]))
        {
            throw CommandException(_T("文件名不能包含非法字符\n"));
        }
        p = join(p, path_name[i]);
        if (!vfs->isExist(p))
        {
            DirHandler ret = vfs->createDir(p);
            if (!ret.isValid())
                return -1;
        }
    }
    return 0;
}