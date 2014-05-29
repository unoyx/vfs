#include "CommandFactory.h"
#include <stdio.h>
#include "SmartPtr.h"
#include "MyString.h"
#include "CommandParser.h"
#include "CommandInterface.h"
#include "CommandPrompt.h"
#include "CommandChangeVolumn.h"
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

SmartPtr<CommandInterface> CommandFactory::create(const MyString& command)
{
    MyString name;
    Vector<MyString> pathes;
    Vector<MyString> switches;
    int flag = parse(command, &name, &pathes, &switches);

    if (name.isEmpty())
    {
        if (pathes.size() == 1
            && _istalpha(pathes[0][0])
            && pathes[0][1] == _T(':'))
        {
            name = _T("change_volumn");
        }
        if (flag == -1)
        {
            throw CommandException(_T("无法识别的命令\n"));
        }
    }
    name = name.toLower();
    for (int i = 0; i < switches.size(); ++i)
    {
        switches[i] = switches[i].toLower();
    }
    SmartPtr<CommandInterface> ret;
    if (name == _T("prompt"))
    {
        ret = SmartPtr<CommandInterface>(new CommandPrompt);
    }
    else if (name == _T("change_volumn"))
    {
        ret = SmartPtr<CommandInterface>(new CommandChangeVolumn);
    }
    else if (name == _T("cd"))
    {
        ret = SmartPtr<CommandInterface>(new CommandCd);
    }
    else if (name == _T("dir"))
    {
        ret = SmartPtr<CommandInterface>(new CommandDir);
    }
    else if (name == _T("mkdir"))
    {
        ret = SmartPtr<CommandInterface>(new CommandMkdir);
    }
    else if (name == _T("rmdir"))
    {
        ret = SmartPtr<CommandInterface>(new CommandRmdir);
    }
    else if (name == _T("copy"))
    {
        ret = SmartPtr<CommandInterface>(new CommandCopy);
    }
    else if (name == _T("del"))
    {
        ret = SmartPtr<CommandInterface>(new CommandDel);
    }
    else if (name == _T("compare"))
    {
        ret = SmartPtr<CommandInterface>(new CommandCompare);
    }
    else
    {
        throw CommandException(_T("无法识别的命令\n"));
    }
    ret->setPathes(pathes);
    ret->setSwitches(switches);
    return ret;
}
