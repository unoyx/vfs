#include "CommandRmdir.h"
#include <stdio.h>
#include "path.h"
#include "state.h"
#include "VirtualDiskNode.h"
#include "DirHandler.h"
#include "DirIterator.h"
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
    if (pathes.size() != 0)
    {
        m_pathes = pathes;
    }
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
    if (m_pathes.size() == 0 && m_recursive)
    {
        MyString path = vfs->pwd();
        if (basename(path).isEmpty())
        {
            m_pathes.append(path);
        }
        else
        {
            vfs->deleteDir(path);
            vfs->chdir(dirname(path));
            return;
        }
    }
    for (int i = 0; i < m_pathes.size(); ++i)
    {
        MyString path = m_pathes[i];
        if (!isNormalizedPath(path))
        {
            path = vfs->pathNormalize(path);
        }
        if (!vfs->isExist(path) || !vfs->isDir(path))
        {
            throw CommandException(_T("系统找不到指定的目录\n"));
        }
        if (m_recursive)
        {
            _tprintf(_T("删除目录%s 是否确认<Y/N>?"), m_pathes[i].c_str());
            TCHAR comfirm = false;
            _tscanf_s(_T("%c%*c"), &comfirm, sizeof(comfirm));
            if (comfirm != _T('Y') && comfirm != _T('y'))
                continue;
            MyString dir = dirname(path);
            MyString base = basename(path);
            if (base.isEmpty())
            {
                for (DirIterator iter = vfs->openDir(dir).getIterator(); !iter.isDone(); iter.next())
                {
                    state s = iter.getItem();
                    if (s.name.startWith(_T(".")))
                        continue;
                    if (s.type == DIR_TYPE)
                    {
                        vfs->deleteDir(s.path);
                    }
                    else if(s.type == FILE_TYPE)
                    {
                        vfs->deleteFile(s.path);
                    }
                }
            }
            else
            {
                vfs->deleteDir(path);
            }
        }
        else
        {
            DirHandler dir = vfs->openDir(path);
            if (dir.isEmpty() && !basename(path).isEmpty())
            {
                dir.close();
                vfs->deleteDir(path);
                if (path == vfs->pwd())
                {
                    vfs->chdir(dirname(path));
                }
            }
            else
            {
                throw CommandException(_T("目录不是空的\n"));
            }
        }
    }
    return;
}
