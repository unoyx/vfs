#ifndef _VIRTUAL_FILE_SYSTEM_H_
#define _VIRTUAL_FILE_SYSTEM_H_
#include "IVirtualDiskProxy.h"
#include "DirNode.h"
#include "CommandFactory.h"

template<typename T>
class Vector;
class FileHandler;
class DirHandler;
class VolumnNode;

class VirtualDiskNode : public IVirtualDiskProxy
{
public:
    VirtualDiskNode(void);
    ~VirtualDiskNode(void);

    virtual int ExecCommand(const char* command); //返回1表示成功，0表示失败
    virtual int GetVolumnCount(void);
    virtual IVolumnProxy* GetVolumnByIdx(int i);

    VolumnNode* addVolumn(MyString label);
    void removeVolumn(int i);
    int findVolumn(MyString label) const;
    VolumnNode* getVolumnRef(int i);

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
    MyString pwd(void) const;
    int changeVolumn(MyString path);
    MyString pwv(void) const;
    MyString pathNormalize(MyString path) const;
private:
    VirtualDiskNode(const VirtualDiskNode&);
    VirtualDiskNode& operator=(const VirtualDiskNode&);

    /* 输入已被规范化的路径名 */
    DirNode* getDirNode(const MyString& path);

    // 当前所工作的分区
    MyString m_pwv;
    
    CommandFactory factory;
    Vector<VolumnNode> m_volumns;
};
#endif // _VIRTUAL_FILE_SYSTEM_H_