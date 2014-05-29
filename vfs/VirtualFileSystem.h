#ifndef _VIRTUAL_FILE_SYSTEM_H_
#define _VIRTUAL_FILE_SYSTEM_H_
#include "DirNode.h"

template<typename T>
class Vector;
class FileHandler;
class DirHandler;

class VirtualDiskNode
{
public:
    VirtualDiskNode(void);
    ~VirtualDiskNode(void);

    FileHandler createFile(MyString path);
    DirHandler createDir(MyString path);
    FileHandler openFile(MyString path);
    DirHandler openDir(MyString path);
    void deleteFile(MyString path);
    void deleteDir(MyString path);

    /* 判断路径是否存在相应的节点 */
    bool isExist(MyString path) const;

    bool isFile(MyString path) const;
    bool isDir(MyString path) const;

    int chdir(MyString path);
    MyString pathNormalize(MyString path) const;
    MyString pwd(void) const;
private:
    VirtualDiskNode(const VirtualDiskNode&);
    VirtualDiskNode& operator=(const VirtualDiskNode&);

    /* 输入已被规范化的路径名 */
    DirNode* getDirNode(const MyString& path);

    DirNode m_root;
    MyString m_pwd;
};
#endif // _VIRTUAL_FILE_SYSTEM_H_