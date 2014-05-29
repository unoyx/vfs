#ifndef _APP_H_
#define _APP_H_
#include "VirtualDisk.h"

class App
{
public:
    App(void);
    ~App(void);

    void run(void);
private:
    IVirtualDiskProxy* m_vdisk;
    // 每行命令的最大长度
    const static int MAX_CMD_SIZE;
};

#endif