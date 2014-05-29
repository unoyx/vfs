#ifndef _COMMAND_CHANGE_VOLUMN_H_
#define _COMMAND_CHANGE_VOLUMN_H_
#include "CommandInterface.h"
#include "MyString.h"

template<typename T>
class Vector;
class VirtualDiskNode;

class CommandChangeVolumn : public CommandInterface
{
public:
    CommandChangeVolumn(void);
    ~CommandChangeVolumn(void);

    virtual void setPathes(const Vector<MyString>& pathes);
    virtual void setSwitches(const Vector<MyString>& switches);
    virtual void exec(VirtualDiskNode* vfs);
private:
    MyString m_volumn;
};
#endif // _COMMAND_CHANGE_VOLUMN_H_

