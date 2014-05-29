#include "FileNode.h"
#include "State.h"

FileNode::FileNode(void)
    :VfsNode(FILE_TYPE)
{
}

FileNode::~FileNode(void)
{
}

state FileNode::stat(void) const
{
    state ret = VfsNode::stat();
    ret.size = getSize();
    return ret;
}

int FileNode::getSize(void) const
{
    return m_content.size();
}
