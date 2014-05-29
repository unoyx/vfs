#include "DirNode.h"
#include <assert.h>
#include "FileNode.h"
#include "state.h"

DirNode::DirNode(void)
    :VfsNode(DIR_TYPE)
{
}

DirNode::~DirNode(void)
{
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