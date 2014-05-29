#include "DirNode.h"
#include <assert.h>
#include "state.h"
#include "path.h"
#include "FileNode.h"
#include "FindResult.h"

DirNode::DirNode(void)

    :VfsNode(DIR_TYPE)
{
}

DirNode::~DirNode(void)
{
}

const char* DirNode::GetName()
{
    return get_name().c_str();
}

ETYPE DirNode::GetType()
{
    return IDir;
}

IFindResult* DirNode::Find(const char* findstr,bool bRecursion)
{
    FindResult* result = new FindResult;
    find(findstr, bRecursion, result);
    return result;
}

// 当前比较未忽略大小写
void DirNode::find(const MyString& findstr, bool recursion, FindResult* result)
{
    if (!result)
        return;

    for (int i = 0; i < m_files.size(); ++i)
    {
        MyString name = m_files[i].stat().name;
        if (match(name, findstr))
        {
            result->add(&m_files[i]);
        }
    }

    for (int i = 0; i < m_dirs.size(); ++i)
    {
        MyString name = m_dirs[i].stat().name;
        if (match(name, findstr))
        {
            result->add(&m_dirs[i]);
        }
        if (recursion)
        {
            m_dirs[i].find(findstr, recursion, result);
        }
    }    
}

state DirNode::stat(void) const
{
    state ret = VfsNode::stat();
    ret.size = 0;
    return ret;
}

FileNode* DirNode::addFile(void)
{
    m_files.append(FileNode());
    return &(m_files[m_files.size() - 1]);
}

void DirNode::removeFile(int pos)
{
    assert(pos >= 0 && pos < m_files.size());
    m_files.remove(pos);
}

int DirNode::findFile(const MyString& name)
{
    for (int i = 0; i < m_files.size(); ++i)
    {
        /* 查找文件时不区分大小写，但创建文件时可区分大小写 */
        if (m_files[i].get_name().toLower() == name)
            return i;
    }
    return -1;
}

FileNode* DirNode::getFileRef(int pos)
{
    return &m_files[pos];
}

DirNode* DirNode::addSubDir(void)
{
    m_dirs.append(DirNode());
    return &(m_dirs[m_dirs.size() - 1]);
}

void DirNode::removeSubDir(int pos)
{
    assert(pos >= 0 && pos < m_dirs.size());
    m_dirs.remove(pos);
}

int DirNode::findSubDir(const MyString& name)
{
    for (int i = 0; i < m_dirs.size(); ++i)
    {
        if (m_dirs[i].get_name().toLower() == name)
            return i;
    }
    return -1;
}

DirNode* DirNode::getSubDirRef(int pos)
{
    return &m_dirs[pos];
}
