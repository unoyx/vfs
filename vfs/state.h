#ifndef _STATE_H_
#define _STATE_H_
#include <time.h>
#include "MyString.h"
#include "VfsNode.h"

struct state
{
    MyString name;
    MyString path;
    time_t ctime;
    NodeType type;
    int size;

    state(void)
        :ctime(0),
        type(INVALID_TYPE),
        size(0)
    {}
};

#endif