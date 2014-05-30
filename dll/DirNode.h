#ifndef _DIR_NODE_H_
#define _DIR_NODE_H_
#include "VfsNode.h"
#include "IVirtualDiskProxy.h"
#include "Vector.h"

struct state;
class DirNode;
class FileNode;
class DirHandler;
class FindResult;

class DirNode : public VfsNode, public IDirProxy
{
public:
    DirNode(void);
    ~DirNode(void);

    virtual const char* GetName();
    virtual ETYPE  GetType();
	virtual IFindResult* Find(const char* findstr,bool bRecursion);
    void find(MyString findstr, bool recursion, FindResult* result);

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
};

#endif

