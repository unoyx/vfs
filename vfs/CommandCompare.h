#ifndef _CommandCompare_h_
#define _CommandCompare_h_
#include "CommandInterface.h"
#include "MyString.h"

template<typename T>
class Vector;
class VirtualDiskNode;

class CommandCompare : public CommandInterface
{
public:
    CommandCompare(void);
    ~CommandCompare(void);

    virtual void setPathes(const Vector<MyString>& pathes);
    virtual void setSwitches(const Vector<MyString>& switches);
    virtual void exec(VirtualDiskNode* vfs);
private:
    MyString m_src;
    MyString m_dst;
    // 文件出现差异时，输出前16个字符及出现差异的字符
    static const int OUTPUT_BYTE;
};

#endif // _CommandCompare_h_



