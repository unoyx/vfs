#include "DirHandler.h"
#include "state.h"
#include "DirIterator.h"

DirHandler::DirHandler(DirNode *dir)
    :m_dir(dir)
{
}

DirHandler::~DirHandler(void)
{
}
DirHandler::DirHandler(const DirHandler& handler)
{
    m_dir = handler.m_dir;
}

DirHandler& DirHandler::operator=(const DirHandler& handler)
{
    if (m_dir != handler.m_dir)
    {
        m_dir = handler.m_dir;
    }
    return *this;
}
bool DirHandler::isValid(void)
{
    return m_dir != nullptr;
}

bool DirHandler::isEmpty(void)
{
    bool ret = true;
    if (!m_dir->m_files.isEmpty())
    {
        ret = false;
    }
    // 除去一定存在的“.”，“..”两个文件夹
    if (m_dir->m_dirs.size() > 2)
    {
        ret = false;
    }
    return ret;
}

void DirHandler::close()
{
    m_dir = nullptr;
}

state DirHandler::stat(void)
{
    return m_dir->stat();
}

DirIterator DirHandler::getIterator(void)
{
    return DirIterator(m_dir->m_dirs, m_dir->m_files);
}
