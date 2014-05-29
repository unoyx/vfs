#ifndef _FIND_RESULT_H_
#define _FIND_RESULT_H_
#include "IVirtualDiskProxy.h"
#include "Vector.h"

class FindResult : public IFindResult
{
public:
    FindResult(void);
    ~FindResult(void);

    virtual void Release();
    virtual unsigned int GetCount();   
    virtual IDiskObj* GetItemByIdx(unsigned int i);

    void add(IDiskObj* ref);
    void merge(const FindResult& rhs);
private:
    FindResult(const FindResult&);
    FindResult& operator=(const FindResult&);

    Vector<IDiskObj*> m_data;
};

#endif // _FIND_RESULT_H_

