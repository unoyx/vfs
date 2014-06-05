#include "CommandCd.h"
#include <assert.h>
#include <stdio.h>
#include "path.h"
#include "Vector.h"
#include "VirtualDiskNode.h"
#include "CommandException.h"

CommandCd::CommandCd(void)
{
}

CommandCd::~CommandCd(void)
{
}

void CommandCd::setPathes(const Vector<MyString>& pathes)
{
    if (pathes.size() > 1)
    {
        throw CommandException(_T("命令语法不正确\n"));
    }
    if (pathes.isEmpty())
        return;
    m_path = pathes[0];
    return;
}

void CommandCd::setSwitches(const Vector<MyString>& switches)
{
    if (switches.size() > 0)
    {
        throw CommandException(_T("命令语法不正确\n"));
    }
    return;
}

void CommandCd::exec(VirtualDiskNode* vfs)
{
    if (!vfs)
    {
        assert(0);
        return;
    }
    if (m_path.isEmpty()) 
    {
        MyString pwd = vfs->pwd();
        if (!pwd.endWith(_T("\\")))
        {
            pwd += _T("\\");
        }
        _tprintf(_T("%s\n"), pwd.c_str());
        return;
    }

    if (!isNormalizedPath(m_path))
    {
        m_path = vfs->pathNormalize(m_path);
    }
    m_path = m_path.toLower();

    if (!vfs->isDir(m_path))
    {
        throw CommandException(_T("系统找不到指定的目录\n"));
    }
    vfs->chdir(m_path);
    return;
}

