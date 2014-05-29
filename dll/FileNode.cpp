#include "FileNode.h"
#include "State.h"

FileNode::FileNode(void)
    :VfsNode(FILE_TYPE)
{
}

FileNode::~FileNode(void)
{
}

const char* FileNode::GetName()
{
    return get_name().c_str();
}

ETYPE FileNode::GetType()
{
    return IFile;
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
