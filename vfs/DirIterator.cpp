#include "DirIterator.h"
#include <assert.h>
#include "DirNode.h"
#include "FileNode.h"
#include "State.h"

DirIterator::DirIterator(const Vector<DirNode>& dir, const Vector<FileNode>& file)
    :m_pos(0)
{
    for (int i = 0; i < dir.size(); ++i)
    {
        state s = dir[i].stat();
        m_states.append(s);
    }
    for (int i = 0; i < file.size(); ++i)
    {
        state s = file[i].stat();
        m_states.append(s);
    }
    for (int i = 2; i < m_states.size(); ++i)
    {
        for (int j = i + 1; j < m_states.size(); ++j)
        {
            if (m_states[i].name > m_states[j].name)
            {
                state t = m_states[i];
                m_states[i] = m_states[j];
                m_states[j] = t;
            }
        }
    }
}

DirIterator::~DirIterator(void)
{
}

bool DirIterator::isDone(void)
{
    return m_pos == m_states.size();
}

void DirIterator::next(void)
{
    assert(m_pos < m_states.size());
    ++m_pos;
}

state DirIterator::getItem(void)
{
    return m_states[m_pos];
}

void DirIterator::reset(void)
{
    m_pos = 0;
}