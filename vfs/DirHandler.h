#ifndef _DIR_HANDLER_H_
#define _DIR_HANDLER_H_
#include "DirNode.h"

struct state;
class DirIterator;

class DirHandler
{
public:
    explicit DirHandler(DirNode *dir);
    ~DirHandler(void);
    DirHandler(const DirHandler& handler);
    DirHandler& operator=(const DirHandler& handler);

    bool isValid(void);
    bool isEmpty(void);
    void close(void);
    state stat(void);
    DirIterator getIterator(void);
private:
    DirNode* m_dir;
};

#endif
