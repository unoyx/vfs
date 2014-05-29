#include "FindResult.h"

FindResult::FindResult(void)
{
}

FindResult::~FindResult(void)
{
}

void FindResult::Release()
{
    if (this)
        delete this;
}

unsigned int FindResult::GetCount()
{
    return m_data.size();
}

IDiskObj* FindResult::GetItemByIdx(unsigned int i)
{
    return m_data[i];
}

void FindResult::add(IDiskObj* ref)
{
    m_data.append(ref);
}

void FindResult::merge(const FindResult& rhs)
{
    for (int i = 0; i < rhs.m_data.size(); ++i)
    {
        m_data.append(rhs.m_data[i]);
    }
}
