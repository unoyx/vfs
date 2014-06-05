#include "VirtualDiskNode.h"
#include <assert.h>
#include <stdio.h>
#include <ctype.h>
#include <tchar.h>
#include "SmartPtr.h"
#include "path.h"
#include "CommandInterface.h"
#include "CommandException.h"
#include "FileHandler.h"
#include "FileNode.h"
#include "DirHandler.h"
#include "VolumnNode.h"

#ifdef _DEBUG
#include "vld.h"
#endif

VirtualDiskNode::VirtualDiskNode(void)
    :m_pwv(_T("c:"))
{
    (void)addVolumn(_T("c:"));
    (void)addVolumn(_T("d:"));
}

VirtualDiskNode::~VirtualDiskNode(void)
{
}

int VirtualDiskNode::ExecCommand(const char* command)
{
    try 
    {
        SmartPtr<CommandInterface> cmd = factory.create(command);
        if (!cmd.isValid())
        {
            throw CommandException(_T("命令创建失败\n"));
        }
        cmd->exec(this);
    }
    catch (const CommandException& e)
    {
        MyString msg = e.get_msg();
        _tprintf(_T("%s"), msg.c_str());
        return 0;
    }
    SmartPtr<CommandInterface> prompt = factory.create(_T("prompt"));
    prompt->exec(this);
    return 1;
}

int VirtualDiskNode::GetVolumnCount(void)
{
    return m_volumns.size();
}

IVolumnProxy* VirtualDiskNode::GetVolumnByIdx(int i)
{
    if (i < 0 || i >= m_volumns.size())
        return nullptr;
    return &m_volumns[i];
}

VolumnNode* VirtualDiskNode::addVolumn(MyString label)
{
    m_volumns.append(VolumnNode(label));
    return &m_volumns[m_volumns.size() - 1];
}

void VirtualDiskNode::removeVolumn(int i)
{
    assert(i >= 0 && i < m_volumns.size());
    m_volumns.remove(i);
}

VolumnNode* VirtualDiskNode::getVolumnRef(int i)
{
    assert(i >= 0 && i < m_volumns.size());
    return &m_volumns[i];
}

int VirtualDiskNode::findVolumn(MyString label) const
{
    int i = 0;
    for (; i < m_volumns.size(); ++i)
    {
        if (m_volumns[i].getLabel().toLower() == label)
            return i;
    }
    return -1;
}

FileHandler VirtualDiskNode::createFile(MyString path)
{
    if (!isNormalizedPath(path))
    {
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
    Vector<MyString> dirnames = split(path);
    MyString label = dirnames[0];
    int idx = findVolumn(label);
    if (idx == -1)
        return nullptr;
    DirNode* ret = dynamic_cast<DirNode*>(getVolumnRef(idx)->GetRootDir());
    if (!ret)
    {
        assert(0);
        return nullptr;
    }

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
    path = path.toLower();

    MyString volumn = path.substr(0, 2);
    int idx = findVolumn(volumn);
    if (idx == -1)
    {
        return -1;
    }
    VolumnNode* v = getVolumnRef(idx);
    v->set_pwd(path.substr(2).toLower());

    return 0;
}

MyString VirtualDiskNode::pwd(void) const
{
    int idx = findVolumn(m_pwv);
    if (idx == -1)
    {
        return m_pwv;
    }
    VolumnNode* v = const_cast<VirtualDiskNode*>(this)->getVolumnRef(idx);
    MyString ret = v->get_pwd();
    ret = join(m_pwv, ret);
    return ret;
}

int VirtualDiskNode::changeVolumn(MyString path)
{
    if (path.size() != 2 || !_istalpha(path[0]) || path[1] != _T(':'))
    {
        return -1;
    }
    m_pwv = path.toLower();
    return 0;
}

MyString VirtualDiskNode::pwv(void) const
{
    return m_pwv;
}

// 不改变路径中的大小写情况
MyString VirtualDiskNode::pathNormalize(MyString path) const
{
    if (!isPath(path))
    {
        return path;
    }
    if (isVolumnRelative(path))
    {
        path = join(pwd().substr(0, 2), path);
    }
    else if (isRelative(path))
    {
        if (path == _T("."))
        {
            path = pwd();
        }
        else if (path == _T(".."))
        {
            path = dirname(pwd());
        }
        else if (path.startWith(_T("..\\")))
        {
            path = dirname(pwd()) + path.substr(2);
        } 
        else if (path.startWith(_T(".\\")))
        {
            path = pwd() + path.substr(1);
        }
        // 直接输入名字的情况
        else if (!match(path, _T("?:*")))
        {
            path = join(pwd(), path);
        }
    }
    Vector<MyString> parts = split(path);
    MyString ret = parts[0];
    for (int i = 1; i < parts.size(); ++i)
    {
        ret = join(ret, parts[i]);
    }

    return ret;
}
