#ifndef _COMMAND_COPY_H_
#define _COMMAND_COPY_H_
#include "CommandInterface.h"
#include "MyString.h"

template<typename T>
class Vector;
class VirtualDiskNode;

class CommandCopy : public CommandInterface
{
public:
    CommandCopy(void);
    ~CommandCopy(void);

    virtual void setPathes(const Vector<MyString>& pathes);
    virtual void setSwitches(const Vector<MyString>& switches);
    virtual void exec(VirtualDiskNode* vfs);
private:
    int copyFile(MyString src, MyString dst, VirtualDiskNode* vfs);
    MyString m_src;
    MyString m_dst;
};

#endif // _COMMAND_COPY_H_


