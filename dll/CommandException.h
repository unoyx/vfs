#ifndef _COMMAND_EXCEPTION_H_
#define _COMMAND_EXCEPTION_H_
#include <exception>
#include <tchar.h>
#include "MyString.h"

class CommandException : public std::exception
{
public:
    CommandException(const TCHAR* msg);
    CommandException(const MyString& msg);
    virtual ~CommandException(void);
    MyString get_msg(void) const;
private:
    MyString m_msg;
};

#endif // _COMMAND_EXCEPTION_H_


