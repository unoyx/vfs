#include "App.h"
#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <memory.h>
#include "MyString.h"

const int App::MAX_CMD_SIZE = 2048;

App::App(void)
    :m_vdisk(nullptr)
{
    m_vdisk = GetVirtualDiskProxy();
}

App::~App(void)
{
    ReleaseVirtualDiskProxy(m_vdisk);
    m_vdisk = nullptr;
}

void App::run(void)
{
    TCHAR buf[MAX_CMD_SIZE + 1] = {0};
    for (;;)
    {
        (void)m_vdisk->ExecCommand(_T("prompt"));
        memset(buf, 0, sizeof(buf));
        TCHAR* ret_t = _fgetts(buf, MAX_CMD_SIZE, stdin);
        if (ret_t == NULL && errno != EINVAL)
        {
            assert(0);
            _tprintf(_T("ÃüÁî¶ÁÈ¡´íÎó\n"));
            return;
        }
        MyString line(buf);
        line.term();
        if (line.startWith(_T("exit")))
            return;
        (void)m_vdisk->ExecCommand(line.c_str());
    }
}
