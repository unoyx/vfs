#include "CommandPrompt.h"
#include <stdio.h>
#include <tchar.h>
#include "MyString.h"
#include "Vector.h"
#include "VirtualDiskNode.h"
#include "CommandException.h"

CommandPrompt::CommandPrompt(void)
{
}


CommandPrompt::~CommandPrompt(void)
{
}

void CommandPrompt::setPathes(const Vector<MyString>& pathes)
{
    if (pathes.size() > 0)
    {
        throw CommandException(_T("命令语法不正确\n"));
    }
    return;
}

void CommandPrompt::setSwitches(const Vector<MyString>& switches)
{
    if (switches.size() > 0)
    {
        throw CommandException(_T("命令语法不正确\n"));
    }
    return;
}

void CommandPrompt::exec(VirtualDiskNode* vfs)
{
    _tprintf("%s\\>\n", vfs->pwd().c_str());
}
