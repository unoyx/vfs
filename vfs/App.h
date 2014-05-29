#ifndef _APP_H_
#define _APP_H_
#include "CommandFactory.h"
#include "VirtualFileSystem.h"

class App
{
public:
    App(void);
    ~App(void);

    void run(void);
private:
    CommandFactory factory;
    VirtualDiskNode m_vdisk;
    // ÿ���������󳤶�
    const static int MAX_CMD_SIZE;
};

#endif