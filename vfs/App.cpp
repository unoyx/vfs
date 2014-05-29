#include "App.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <memory.h>
#include "SmartPtr.h"
#include "CommandParser.h"
#include "CommandInterface.h"
#include "CommandException.h"

const int App::MAX_CMD_SIZE = 2048;

App::App(void)
{
}

App::~App(void)
{
}

void App::run(void)
{
    TCHAR buf[MAX_CMD_SIZE + 1] = {0};
    for (;;)
    {
        _tprintf(_T("%s>"), m_vdisk.pwd().c_str());
        memset(buf, 0, sizeof(buf));
        TCHAR* ret_t = _fgetts(buf, MAX_CMD_SIZE, stdin);
        if (ret_t == NULL && errno != EINVAL)
        {
            assert(0);
            _tprintf(_T("√¸¡Ó∂¡»°¥ÌŒÛ\n"));
            return;
        }
        MyString line(buf);
        MyString name;
        Vector<MyString> pathes;
        Vector<MyString> switches;
        int ret = parse(line, &name, &pathes, &switches);
        if (name.isEmpty() || ret == -1)
        {
            pathes.clear();
            switches.clear();
            continue;
        }
        name = name.toLower();
        for (int i = 0; i < switches.size(); ++i)
        {
            switches[i] = switches[i].toLower();
        }
        if (name == _T("exit"))
        {
            break;
        }
        try 
        {
            SmartPtr<CommandInterface> cmd = factory.create(name);
            if (!cmd.isValid())
            {
                throw CommandException(_T("√¸¡Ó¥¥Ω® ß∞‹\n"));
            }
            cmd->setPathes(pathes);
            cmd->setSwitches(switches);
            cmd->exec(&m_vdisk);
        }
        catch (const CommandException& e)
        {
            MyString msg = e.get_msg();
            _tprintf(_T("%s"), msg.c_str());
            memset(buf, 0, sizeof(buf));
            pathes.clear();
            switches.clear();
        }

    }
}
