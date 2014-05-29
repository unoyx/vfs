#ifndef _COMMAND_CD_H_
#define _COMMAND_CD_H_
#include "CommandInterface.h"
#include "MyString.h"

template<typename T>
class Vector;

class CommandCd : public CommandInterface
{
public:
    CommandCd(void);
    ~CommandCd(void);

    virtual void setPathes(const Vector<MyString>& pathes);
    virtual void setSwitches(const Vector<MyString>& switches);
    virtual void exec(VirtualDiskNode* vfs);
private:
    MyString m_path;
};

#endif //_COMMAND_CD_H_

