#include "VolumnNode.h"
#include "MyString.h"
#include "path.h"

VolumnNode::VolumnNode(void)
    :m_pwd(_T(""))
{
}

VolumnNode::VolumnNode(const MyString& label)
    :m_pwd(_T(""))
{
    setLabel(label);
    DirNode* self = m_root.addSubDir();
    self->set_name(_T("."));
    self->set_path(join(label, _T(".")));
    DirNode* parent = m_root.addSubDir();
    parent->set_name(_T(".."));
    parent->set_path(join(label, _T("..")));
}

VolumnNode::~VolumnNode(void)
{
}

void VolumnNode::setLabel(const MyString& label)
{
    m_root.set_name(label);
}

MyString VolumnNode::getLabel(void) const
{
    return m_root.get_name();
}

IDirProxy* VolumnNode::GetRootDir(void)
{
    return &m_root;
}

void VolumnNode::set_pwd(MyString path)
{
    m_pwd = path;
}

MyString VolumnNode::get_pwd(void) const
{
    return m_pwd;
}
