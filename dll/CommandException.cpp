#include "CommandException.h"

CommandException::CommandException(const TCHAR* msg)
    :m_msg(msg)
{
}

CommandException::CommandException(const MyString& msg)
    :m_msg(msg)
{
}


CommandException::~CommandException(void)
{
}

MyString CommandException::get_msg(void) const
{
    return m_msg;
}
