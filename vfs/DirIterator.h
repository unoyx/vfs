#ifndef _DIR_ITERATOR_H_
#define _DIR_ITERATOR_H_
#include "Vector.h"

struct state;
class DirNode;
class FileNode;

class DirIterator
{
public:
    DirIterator(const Vector<DirNode>& dir, const Vector<FileNode>& file);
    ~DirIterator(void);

    bool isDone(void);
    void next(void);
    state getItem(void);
    void reset(void);
private:
    Vector<state> m_states;
    int m_pos;
};

#endif
