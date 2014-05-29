#include "VirtualFileSystem.h"
#include <assert.h>
#include "path.h"
#include "FileHandler.h"
#include "FileNode.h"
#include "DirHandler.h"

VirtualDiskNode::VirtualDiskNode(void)
    :m_pwd(_T("C:"))
{
    m_root.set_name(_T("C:"));
    DirNode* n_self = m_root.addSubDir();
    n_self->set_name(_T("."));
    n_self->set_path(m_root.get_name() + _T("\\."));
    DirNode* n_parent = m_root.addSubDir();
    n_parent->set_name(_T(".."));
    n_parent->set_path(m_root.get_name() + _T("\\.."));
}

VirtualDiskNode::~VirtualDiskNode(void)
{
}

FileHandler VirtualDiskNode::createFile(MyString path)
{
    if (!isNormalizedPath(path))
    {
        assert(0);
        return FileHandler(nullptr);
    }
    MyString dir_path = dirname(path).toLower();
    DirNode* parent = getDirNode(dir_path);
    if (!parent)
        return FileHandler(nullptr);
    // 文件名创建时保留大小写
    MyString file_name = basename(path);
    if (parent->findFile(file_name) != -1)
    {
        return FileHandler(nullptr);
    }

    FileNode* nfile = parent->addFile();
    assert(nfile);
    nfile->set_name(file_name);
    nfile->set_path(path.toLower());
    return FileHandler(nfile);
}

DirHandler VirtualDiskNode::createDir(MyString path)
{
    if (!isNormalizedPath(path))
    {
        assert(0);
        return DirHandler(nullptr);
    }
    MyString dir_path = dirname(path).toLower();
    DirNode* parent = getDirNode(dir_path);
    if (!parent)
        return DirHandler(nullptr);

    MyString dir_name = basename(path);
    if (dir_name == _T("..") || dir_name == _T("."))
        return DirHandler(nullptr);
    if (parent->findSubDir(dir_name.toLower()) != -1)
        return DirHandler(nullptr);

    DirNode* ndir = parent->addSubDir();
    assert(ndir);
    ndir->set_name(dir_name);
    ndir->set_path(path);
    // 新建文件夹中，包含“.”，“..”两个文件夹。
    DirNode* n_self = ndir->addSubDir();
    n_self->set_name(_T("."));
    n_self->set_path(dir_path + _T("\\."));
    DirNode* n_parent = ndir->addSubDir();
    n_parent->set_name(_T(".."));
    n_parent->set_path(dir_path + _T("\\.."));
    n_parent->set_ctime(parent->get_ctime());
    return DirHandler(ndir);
}

FileHandler VirtualDiskNode::openFile(MyString path)
{
    if (!isNormalizedPath(path))
    {
        assert(0);
        return FileHandler(nullptr);
    }
    path = path.toLower();
    MyString dir_path = dirname(path);
    DirNode* parent = getDirNode(dir_path);
    if (!parent)
        return FileHandler(nullptr);
    MyString file_name = basename(path);
    // 文件至少被存放在根目录之下
    if (file_name.isEmpty())
    {
        return FileHandler(nullptr);
    }
    int file = parent->findFile(file_name);
    if (file == -1)
    {
        return FileHandler(nullptr);
    }
    else
    {
        return FileHandler(parent->getFileRef(file));
    }
}

DirHandler VirtualDiskNode::openDir(MyString path)
{
    if (!isNormalizedPath(path))
    {
        assert(0);
        return DirHandler(nullptr);
    }
    path = path.toLower();
    MyString dir_path = dirname(path);
    DirNode* parent = getDirNode(dir_path);
    if (!parent)
        return DirHandler(nullptr);
    MyString dir_name = basename(path);
    // 访问根目录, “空”文件夹的情况
    if (dir_name.isEmpty() || dir_name == _T("..") || dir_name == _T("."))
    {
        return DirHandler(parent);
    }
    int dir = parent->findSubDir(dir_name);
    if (dir == -1)
    {
        return DirHandler(nullptr);
    }
    else
    {
        return DirHandler(parent->getSubDirRef(dir));
    }
}

void VirtualDiskNode::deleteFile(MyString path)
{
    if (!isNormalizedPath(path))
    {
        assert(0);
        return;
    }
    path = path.toLower();
    MyString dir_path = dirname(path);
    DirNode* parent = getDirNode(dir_path);
    if (!parent)
        return;
    MyString file_name = basename(path);
    assert(!file_name.isEmpty());
    int file = parent->findFile(file_name);
    if (file != -1)
    {
        parent->removeFile(file);
    }
    return;
}

void VirtualDiskNode::deleteDir(MyString path)
{
    if (!isNormalizedPath(path))
    {
        assert(0);
        return;
    }
    path = path.toLower();
    MyString dir_path = dirname(path);
    DirNode* parent = getDirNode(dir_path);
    if (!parent)
        return;
    MyString dir_name = basename(path);
    assert(!dir_name.isEmpty());
    int dir = parent->findSubDir(dir_name);
    if (dir != -1)
    {
        parent->removeSubDir(dir);
    }
    return;
}

bool VirtualDiskNode::isExist(MyString path) const
{
    if (!isNormalizedPath(path))
    {
        assert(0);
        return false;
    }
    path = path.toLower();
    MyString dir_path = dirname(path);
    DirNode* parent = const_cast<VirtualDiskNode*>(this)->getDirNode(dir_path);
    if (!parent)
        return false;
    MyString name = basename(path);
    if (name.isEmpty())
        return true;

    int file = parent->findFile(name);
    if (file != -1)
        return true;
    int dir = parent->findSubDir(name);
    if (dir != -1)
        return true;
    return false;
}

bool VirtualDiskNode::isDir(MyString path) const
{
    if (!isNormalizedPath(path))
    {
        assert(0);
        return false;
    }
    path = path.toLower();
    MyString dir_path = dirname(path);
    DirNode* parent = const_cast<VirtualDiskNode*>(this)->getDirNode(dir_path);
    if (!parent)
        return false;
    MyString name = basename(path);
    if (name.isEmpty())
        return true;
    int dir = parent->findSubDir(name);
    return dir != -1;
}

bool VirtualDiskNode::isFile(MyString path) const
{
    if (!isNormalizedPath(path))
    {
        assert(0);
        return false;
    }
    path = path.toLower();
    MyString dir_path = dirname(path);
    DirNode* parent = const_cast<VirtualDiskNode*>(this)->getDirNode(dir_path);
    if (!parent)
        return false;
    MyString name = basename(path);
    int file = parent->findFile(name);
    return file != -1;
}

DirNode* VirtualDiskNode::getDirNode(const MyString& path)
{
    assert(isNormalizedPath(path));
    Vector<MyString> dirnames = split(path);
    DirNode* ret = &m_root;
    if (dirnames[0] != ret->get_name().toLower() || ret->get_type() == FILE_TYPE)
        return nullptr;

    for (int i = 1; i < dirnames.size(); ++i)
    {
        int subdir = ret->findSubDir(dirnames[i]);
        if (subdir == -1)
        {
            return nullptr;
        }
        else
        {
            ret = ret->getSubDirRef(subdir);
        }
    }
    return ret;
}

int VirtualDiskNode::chdir(MyString path)
{
    if (!isNormalizedPath(path))
    {
        path = pathNormalize(path);
    }
    path[0] = _T('C');
    m_pwd = path;
    return 0;
}

// 不改变路径中的大小写情况
MyString VirtualDiskNode::pathNormalize(MyString path) const
{
    MyString ret;
    if (!isPath(path))
    {
        assert(0);
        return ret;
    }
    if (isVolumnRelative(path))
    {
        ret = join(m_pwd.substr(0, 2), path);
    }
    else if (isRelative(path))
    {
        if (path == _T("."))
        {
            ret = m_pwd;
        }
        else if (path == _T(".."))
        {
            ret = dirname(m_pwd);
        }
        else if (path.startWith(_T("..\\")))
        {
            ret = dirname(m_pwd) + path.substr(2);
        } 
        else if (path.startWith(_T(".\\")))
        {
            ret = m_pwd + path.substr(1);
        }
        // 直接输入名字的情况
        else if (!match(path, _T("?:*")))
        {
            ret = m_pwd + _T("\\") + path;
        }
    } 
    // 绝对路径中处理：\dir\file的情况
    //else if (path.startWith(_T("\\")))
    //{
    //    ret = _T("c:") + path;
    //}

    if (path.endWith(_T("\\")))
    {
        ret = ret.substr(0, ret.size() - 1);
    }
    return ret;
}

MyString VirtualDiskNode::pwd(void) const
{
    return m_pwd;
}
