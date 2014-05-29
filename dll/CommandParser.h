#ifndef _COMMAND_PARSER_H_
#define _COMMAND_PARSER_H_

class MyString;
template<typename T>
class Vector;

int parse(const MyString& cmd, MyString* name, Vector<MyString>* pathes, Vector<MyString>* switches);

#endif // _COMMAND_PARSER_H_
