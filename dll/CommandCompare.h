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
    // �ļ����ֲ���ʱ�����ǰ16���ַ������ֲ�����ַ�
    static const int OUTPUT_BYTE;
};

#endif // _CommandCompare_h_



