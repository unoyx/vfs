#ifndef _COMMAND_DIR_H_
#define _COMMAND_DIR_H_
#include "CommandInterface.h"
#include "Vector.h"

class MyString;

class CommandDir : public CommandInterface
{
public:
    CommandDir(void);
    ~CommandDir(void);

    virtual void setPathes(const Vector<MyString>& pathes);
    virtual void setSwitches(const Vector<MyString>& switches);
    virtual void exec(VirtualDiskNode* vfs);
private:
    void listDir(const MyString& path, VirtualDiskNode* vfs, bool only_dir = false);
    void listDirRecursive(const MyString& path, VirtualDiskNode* vfs, bool only_dir = false);
    void displayFileSize(int n, int length = 0);

    Vector<MyString> m_pathes;
    bool m_only_dir;
    bool m_recursive;
};

#endif // _COMMAND_DIR_H_