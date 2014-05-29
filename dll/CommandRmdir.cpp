#include "CommandRmdir.h"
#include <stdio.h>
#include "path.h"
#include "VirtualDiskNode.h"
#include "DirHandler.h"
#include "CommandException.h"

CommandRmdir::CommandRmdir(void)
    :m_recursive(false)
{
}

CommandRmdir::~CommandRmdir(void)
{
}

void CommandRmdir::setPathes(const Vector<MyString>& pathes)
{
    if (pathes.size() == 0)
    {
        throw CommandException(_T("命令语法不正确\n"));
    }
    m_pathes = pathes;
    return;
}

void CommandRmdir::setSwitches(const Vector<MyString>& switches)
{
    for (int i = 0; i < switches.size(); ++i)
    {
        if (switches[i] == _T("/s"))
        {
            m_recursive = true;
        }
        else
        {
            throw CommandException(_T("无效的开关") + switches[i]);
        }
    }
    return;
}

void CommandRmdir::exec(VirtualDiskNode* vfs)
{
    for (int i = 0; i < m_pathes.size(); ++i)
    {
        MyString path = m_pathes[i];
        if (!isNormalizedPath(path))
        {
            path = vfs->pathNormalize(path);
        }
        if (vfs->pwd().startWith(path))
        {
            _tprintf(_T("另一个程序正在使用此文件，进程无法访问。\n"));
            continue;
        }
        if (!vfs->isExist(path) || !vfs->isDir(path))
        {
            _tprintf(_T("系统找不到指定的目录\n"));
            continue;
        }
        if (m_recursive)
        {
            _tprintf(_T("%s, 是否确认<Y/N>?"), m_pathes[i].c_str());
            TCHAR comfirm = false;
            _tscanf_s(_T("%c%*c"), &comfirm, sizeof(comfirm));
            if (comfirm == 'Y' || comfirm == 'y')
            {
                vfs->deleteDir(path);
            }
        }
        else
        {
            DirHandler dir = vfs->openDir(path);
            if (dir.isEmpty())
            {
                dir.close();
                vfs->deleteDir(path);
            }
            else
            {
                _tprintf(_T("目录不是空的\n"));
            }
        }
    }
    return;
}
