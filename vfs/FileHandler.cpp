#include "FileHandler.h"
#include <assert.h>
#include "Vector.h"
#include "FileNode.h"
#include "State.h"

FileHandler::FileHandler(FileNode* file)
    :m_file(file)
{
}

FileHandler::~FileHandler(void)
{
}

FileHandler::FileHandler(const FileHandler& handler)
{
    m_file = handler.m_file;
}

FileHandler& FileHandler::operator=(const FileHandler& handler)
{
    if (m_file != handler.m_file)
    {
        m_file = handler.m_file;
    }
    return *this;
}

bool FileHandler::isValid(void)
{
    return m_file != nullptr;
}

void FileHandler::close(void)
{
    m_file = nullptr;
}

state FileHandler::stat(void)
{
    return m_file->stat();
}

Vector<char> FileHandler::read(void)
{
    return m_file->m_content;
}

void FileHandler::write(const char* buf, int len)
{
    m_file->m_content.clear();
    for (int i = 0; i < len; ++i)
    {
        m_file->m_content.append(buf[i]);
    }
}

