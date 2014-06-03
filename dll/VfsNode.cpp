#include "VfsNode.h"
#include <time.h>
#include "state.h"

VfsNode::VfsNode(NodeType t)
    :m_type(t)
{
    m_ctime = time(nullptr);
}

VfsNode::~VfsNode(void)
{
}

state VfsNode::stat(void) const
{
    state ret;
    ret.name = m_name;
    ret.path = get_path();
    ret.ctime = get_ctime();
    ret.type = get_type();
    return ret;
}
