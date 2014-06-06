#ifndef _COMMAND_FACTORY_H_
#define _COMMAND_FACTORY_H_

class MyString;
class CommandInterface;
template <typename T>
class SmartPtr;

class CommandFactory
{
public:
    CommandFactory(void);
    ~CommandFactory(void);
    static SmartPtr<CommandInterface> create(const MyString& cmd_name);
};

#endif