#ifndef _VOLUMN_NODE_H_
#define _VOLUMN_NODE_H_
#include "IVirtualDiskProxy.h"
#include "MyString.h"
#include "DirNode.h"

class VolumnNode : public IVolumnProxy
{
public:
    VolumnNode(void);
    VolumnNode(const MyString& label);
    ~VolumnNode(void);

    virtual IDirProxy* GetRootDir(void);

    void setLabel(const MyString& label);
    MyString getLabel(void) const;
    void set_pwd(MyString path);
    MyString get_pwd(void) const;
private:
    MyString m_pwd;
    DirNode m_root;
};
#endif // _VOLUMN_NODE_H_

