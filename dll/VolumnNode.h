#ifndef _VOLUMN_NODE_H_
#define _VOLUMN_NODE_H_
#include "IVirtualDiskProxy.h"
#include "DirNode.h"

class MyString;

class VolumnNode : public IVolumnProxy
{
public:
    VolumnNode(void);
    VolumnNode(const MyString& label);
    ~VolumnNode(void);

    virtual IDirProxy* GetRootDir(void);

    void setLabel(const MyString& label);
    MyString getLabel(void);
private:
    DirNode m_root;
};
#endif // _VOLUMN_NODE_H_

