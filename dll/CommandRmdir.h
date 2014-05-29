#ifndef _CommandRmdir_h_
#define _CommandRmdir_h_
#include "CommandInterface.h"
#include "Vector.h"
#include "MyString.h"

class CommandRmdir : public CommandInterface
{
public:
    CommandRmdir(void);
    ~CommandRmdir(void);

    virtual void setPathes(const Vector<MyString>& pathes);
    virtual void setSwitches(const Vector<MyString>& switches);
    virtual void exec(VirtualDiskNode* vfs);
private:
    Vector<MyString> m_pathes;
    bool m_recursive;
};

#endif // _CommandRmdir_h_
