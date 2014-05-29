#ifndef _COMMAND_DEL_H_
#define _COMMAND_DEL_H_
#include "CommandInterface.h"
#include "Vector.h"
#include "MyString.h"

class CommandDel : public CommandInterface
{
public:
    CommandDel(void);
    ~CommandDel(void);

    virtual void setPathes(const Vector<MyString>& pathes);
    virtual void setSwitches(const Vector<MyString>& switches);
    virtual void exec(VirtualDiskNode* vfs);
private:
    void del(MyString path, VirtualDiskNode* vfs);

    Vector<MyString> m_pathes;
    bool m_recursion;
};

#endif // _COMMAND_DEL_H_
