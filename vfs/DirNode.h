#ifndef _DIR_NODE_H_
#define _DIR_NODE_H_
#include "VfsNode.h"
#include "Vector.h"

struct state;
class DirNode;
class FileNode;
class DirHandler;

class DirNode : public VfsNode
{
public:
    DirNode(void);
    ~DirNode(void);

    state stat(void) const;

    FileNode* addFile(void);
    void removeFile(int pos);
    int findFile(const MyString& name);
    FileNode* getFileRef(int pos);

    DirNode* addSubDir(void);
    void removeSubDir(int pos);
    int findSubDir(const MyString& name);
    DirNode* getSubDirRef(int pos);
private:
    Vector<DirNode> m_dirs;
    Vector<FileNode> m_files;
    friend class DirHandler;
    //friend class VirtualFileSystem;
};

#endif

