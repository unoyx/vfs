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

VirtualDiskNode::VirtualDiskNode(void)
    :m_pwd(_T("C:"))
{
    (void)addVolumn(_T("C:"));
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
            throw CommandException(_T("�����ʧ��\n"));
        }
        cmd->exec(this);
    }
    catch (const CommandException& e)
    {
        MyString msg = e.get_msg();
        _tprintf(_T("%s"), msg.c_str());
        return 0;
    }
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

int VirtualDiskNode::findVolumn(MyString label)
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
        assert(0);
        return FileHandler(nullptr);
    }
    MyString dir_path = dirname(path).toLower();
    DirNode* parent = getDirNode(dir_path);
    if (!parent)
        return FileHandler(nullptr);
    // �ļ�������ʱ������Сд
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
    // �½��ļ����У�������.������..�������ļ��С�
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
    // �ļ����ٱ�����ڸ�Ŀ¼֮��
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
    // ���ʸ�Ŀ¼, ���ա��ļ��е����
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
    path[0] = _T('C');
    m_pwd = path;
    return 0;
}

// ���ı�·���еĴ�Сд���
MyString VirtualDiskNode::pathNormalize(MyString path) const
{
    MyString ret;
    if (!isPath(path))
    {
        assert(0);
        return ret;
    }
    if (isRelative(path))
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
        // ֱ���������ֵ����
        else if (!match(path, _T("?:*")))
        {
            ret = m_pwd + _T("\\") + path;
        }
    } 
    // ����·���д�����\dir\file�����
    else if (path.startWith(_T("\\")))
    {
        ret = _T("c:") + path;
    }

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