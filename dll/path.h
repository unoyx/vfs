#ifndef _PATH_H_
#define _PATH_H_
#include <tchar.h>

class MyString;
template<typename T>
class Vector;

bool hasWildcard(const MyString& s);
bool match(const MyString& src, const MyString& pattern);
bool isAbs(MyString path);
bool isRelative(MyString path);
bool isPath(MyString path);
bool isNormalizedPath(MyString path);
bool isLegalChar(TCHAR c);
bool isLegalName(MyString name);
MyString dirname(MyString path);
MyString basename(MyString path);
Vector<MyString> split(const MyString& path);
MyString join(MyString path, MyString name);

#endif // _PATH_H_
