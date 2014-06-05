#ifndef _FILE_NODE_H_
#define _FILE_NODE_H_
#include "VfsNode.h"
#include "IVirtualDiskProxy.h"
#include "Vector.h"

struct state;
class FileHandler;
//class VirtualFileSystem;

class FileNode : public VfsNode, public IFileProxy
{
public:
    FileNode(void);
    ~FileNode(void);

    virtual const char* GetName();
    virtual ETYPE  GetType();

    state stat(void) const;
    int getSize(void) const;
private:
    Vector<char> m_content;
    friend class FileHandler;
};

#endif



