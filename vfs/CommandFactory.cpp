#include "CommandFactory.h"
#include <stdio.h>
#include "SmartPtr.h"
#include "MyString.h"
#include "CommandInterface.h"
#include "CommandCd.h"
#include "CommandCompare.h"
#include "CommandCopy.h"
#include "CommandDel.h"
#include "CommandDir.h"
#include "CommandMkdir.h"
#include "CommandRmdir.h"
#include "CommandException.h"

CommandFactory::CommandFactory(void)
{
}

CommandFactory::~CommandFactory(void)
{
}

SmartPtr<CommandInterface> CommandFactory::create(const MyString& cmd_name)
{
    if (cmd_name == _T("cd"))
    {
        return SmartPtr<CommandInterface>(new CommandCd);
    }
    else if (cmd_name == _T("dir"))
    {
        return SmartPtr<CommandInterface>(new CommandDir);
    }
    else if (cmd_name == _T("mkdir"))
    {
        return SmartPtr<CommandInterface>(new CommandMkdir);
    }
    else if (cmd_name == _T("rmdir"))
    {
        return SmartPtr<CommandInterface>(new CommandRmdir);
    }
    else if (cmd_name == _T("copy"))
    {
        return SmartPtr<CommandInterface>(new CommandCopy);
    }
    else if (cmd_name == _T("del"))
    {
        return SmartPtr<CommandInterface>(new CommandDel);
    }
    else if (cmd_name == _T("compare"))
    {
        return SmartPtr<CommandInterface>(new CommandCompare);
    }
    else
    {
        throw CommandException(_T("无法识别的命令\n"));
    }
}
