#include "CommandDir.h"
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "state.h"
#include "path.h"
#include "VirtualDiskNode.h"
#include "FileHandler.h"
#include "DirHandler.h"
#include "DirIterator.h"
#include "CommandException.h"

CommandDir::CommandDir(void)
    :m_only_dir(false),
    m_recursive(false)
{
}

CommandDir::~CommandDir(void)
{
}

void CommandDir::setPathes(const Vector<MyString>& pathes)
{
    if (pathes.size() > 0)
    {
        m_pathes = pathes;
    }
    return;
}

void CommandDir::setSwitches(const Vector<MyString>& switches)
{
    for (int i = 0; i < switches.size(); ++i)
    {
        if (switches[i] == _T("/s"))
        {
            m_recursive = true;
        }
        else if (switches[i] == _T("/ad"))
        {
            m_only_dir = true;
        }
        else
        {
            throw CommandException(_T("无效的参数") + switches[i] + _T("\n"));
        }
    }
    return;
}

void CommandDir::exec(VirtualDiskNode* vfs)
{
    if (!vfs)
    {
        assert(0);
        return;
    }
    
    if (m_pathes.isEmpty())
    {
        m_pathes.append(vfs->pwd());
    }

    for (int i = 0; i < m_pathes.size(); ++i)
    {
        MyString path = m_pathes[i];
        if (!isNormalizedPath(path))
        {
            path = vfs->pathNormalize(path);
        }
        path = path.toLower();
        if (!vfs->isExist(path))
        {
            throw CommandException(_T("系统找不到指定路径\n"));
        }
        if (vfs->isFile(path))
        {
            FileHandler file_handler = vfs->openFile(path);
            state s = file_handler.stat();
            displayState(s, false);
            _tprintf(_T("%15d 个文件%14d 字节\n"), 1, s.size);
            _tprintf(_T("%15d 个目录\n"), 0);
        }
        if (vfs->isDir(path))
        {
            if (m_recursive)
            {
                listDirRecursive(path, vfs, m_only_dir);
            }
            else
            {
                listDir(path, vfs, m_only_dir);
            }
        }
    }
    return;
}

void CommandDir::displayState(state s, bool only_dir)
{
    static const int BUF_SIZE = 80;
    char buf[BUF_SIZE] = {0};
    tm t = {0};
    localtime_s(&t, &s.ctime);
    //                            两个空格
    strftime(buf, BUF_SIZE, "%Y/%m/%d  %H:%M", &t);
    printf("%s", buf);

    if (s.type == FILE_TYPE && !only_dir)
    {
        displayFileSize(s.size);
        _tprintf(_T(" %s\n"), s.name.c_str());
    }
    else if (s.type == DIR_TYPE)
    {
         //         三个空格   六个空格
        _tprintf(_T("   <DIR>      %s\n"), s.name.c_str());
    }
}

void CommandDir::listDir(const MyString& path, VirtualDiskNode* vfs, bool only_dir /*= false*/)
{
    DirHandler dir_handler = vfs->openDir(path);
    int file_count = 0;
    int file_size = 0;
    int dir_count = 0;
    for (DirIterator iter = dir_handler.getIterator(); !iter.isDone(); iter.next())
    {
        state s = iter.getItem();
        if (s.type == FILE_TYPE && only_dir)
            continue;
        displayState(s, only_dir);

        if (s.type == FILE_TYPE && !only_dir)
        {
            ++file_count;
            file_size += s.size;
        }
        else if (s.type == DIR_TYPE)
        {
            ++dir_count;
            //         三个空格   六个空格
        }
    }
    _tprintf(_T("%15d 个文件%14d 字节\n"), file_count, file_size);
    _tprintf(_T("%15d 个目录\n"), dir_count);
}

void CommandDir::listDirRecursive(const MyString& path, VirtualDiskNode* vfs, bool only_dir /*= false*/)
{
    _tprintf(_T(" %s 的目录\n\n"), path.c_str());
    listDir(path, vfs, only_dir);
    DirHandler dir_handler = vfs->openDir(path);
    for (DirIterator iter = dir_handler.getIterator(); !iter.isDone(); iter.next())
    {
        state s = iter.getItem();
        if (s.type == DIR_TYPE && !s.name.startWith(_T(".")))
        {
            listDirRecursive(s.path, vfs, only_dir);
        }
    }
}

void CommandDir::displayFileSize(int n, int length /*=0*/)
{
    // 3位一组，每组包含1个逗号分隔符。最多输出18个字符。
    static const int FIELD = 1000;
    static const int FIELD_SIZE = 4;
    static const int MAX_SIZE = 15;
    if (n > FIELD)
    {
        int field = n % 1000;
        displayFileSize(n / 1000, length + FIELD_SIZE);
        printf(",%d", field);
    }
    else
    {
        assert(n >= 0);
        if (n > 0)
            length += static_cast<int>(log10(static_cast<float>(n)));
        else
            length = 0;
        for (int i = 0; i < (MAX_SIZE - (length + FIELD_SIZE - 1)); ++i)
        {
            printf(" ");
        }
        printf("%d", n);
    }
}
