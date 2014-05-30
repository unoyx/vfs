#ifndef _VFS_NODE_H_
#define _VFS_NODE_H_
#include <time.h>
#include "MyString.h"

struct state;

enum NodeType {INVALID_TYPE = 0, DIR_TYPE = 1, FILE_TYPE = 2};

class VfsNode
{
public:
    explicit VfsNode(NodeType t);
    virtual ~VfsNode(void);

    inline void set_name(const MyString& name)
    {
        m_name = name;
    }

    inline MyString get_name(void) const
    {
        return m_name;
    }

    inline void set_path(const MyString& path)
    {
        m_path = path;
    }

    inline MyString get_path(void) const
    {
        return m_path;
    }

    inline NodeType get_type(void) const
    {
        return m_type;
    }

    inline void set_ctime(const time_t& time)
    {
        m_ctime = time;
    }

    inline time_t get_ctime(void) const
    {
        return m_ctime;
    }

    state stat(void) const;
private:

    MyString m_name;
    MyString m_path;
    NodeType m_type;
    /* 该文件/目录的创建时间 */
    time_t m_ctime;
};

#endif