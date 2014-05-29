#ifndef _COMMAND_MKDIR_H_
#define _COMMAND_MKDIR_H_
#include "CommandInterface.h"
#include "Vector.h"
#include "MyString.h"

class CommandMkdir : public CommandInterface
{
public:
    CommandMkdir(void);
    ~CommandMkdir(void);

    virtual void setPathes(const Vector<MyString>& pathes);
    virtual void setSwitches(const Vector<MyString>& switches);
    virtual void exec(VirtualDiskNode* vfs);
private:
    int mkdir(MyString path, VirtualDiskNode* vfs);
    Vector<MyString> m_pathes;
};

#endif
