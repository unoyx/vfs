#ifndef _COMMAND_PROMPT_H_
#define _COMMAND_PROMPT_H_
#include "CommandInterface.h"

template<typename T>
class Vector;
class MyString;
class VirtualDiskNode;

class CommandPrompt : public CommandInterface
{
public:
    CommandPrompt(void);
    ~CommandPrompt(void);

    virtual void setPathes(const Vector<MyString>& pathes);
    virtual void setSwitches(const Vector<MyString>& switches);
    virtual void exec(VirtualDiskNode* vfs);
};
#endif // _COMMAND_PROMPT_H_

