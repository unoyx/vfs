#ifndef _COMMAND_INTERFACE_H_
#define _COMMAND_INTERFACE_H_

template<typename T>
class Vector;
class MyString;
class VirtualDiskNode;

class CommandInterface
{
public:
    virtual void setPathes(const Vector<MyString>& pathes) = 0;
    virtual void setSwitches(const Vector<MyString>& switches) = 0;
    virtual void exec(VirtualDiskNode* vfs) = 0;
    virtual ~CommandInterface(void);
};

#endif