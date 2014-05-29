#ifndef _FILE_NODE_H_
#define _FILE_NODE_H_
#include "VfsNode.h"
#include "Vector.h"

struct state;
class FileHandler;
//class VirtualFileSystem;

class FileNode:public VfsNode
{
public:
    FileNode(void);
    ~FileNode(void);

    state stat(void) const;
    int getSize(void) const;
private:
    Vector<char> m_content;
    friend class FileHandler;
    //friend class VirtualFileSystem;
};

#endif



