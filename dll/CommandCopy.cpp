#include "CommandCopy.h"
#include <Windows.h>
#include <stdio.h>
#include <tchar.h>
#include "path.h"
#include "DelegateMem.h"
#include "VirtualDiskNode.h"
#include "FileHandler.h"
#include "CommandException.h"

CommandCopy::CommandCopy(void)
{
}

CommandCopy::~CommandCopy(void)
{
}

void CommandCopy::setPathes(const Vector<MyString>& pathes)
{
    if (pathes.size() != 2)
    {
        throw CommandException(_T("命令语法不正确\n"));
    }
    m_src = pathes[0];
    if (!isAbs(m_src))
    {
        throw CommandException(_T("命令语法不正确\n"));
    }
    m_dst = pathes[1];
    return;
}

void CommandCopy::setSwitches(const Vector<MyString>& switches)
{
    if (switches.size() != 0)
    {
        throw CommandException(_T("命令语法不正确\n"));
    }
    return;
}

void CommandCopy::exec(VirtualDiskNode* vfs)
{
    if (!isNormalizedPath(m_dst))
    {
        m_dst = vfs->pathNormalize(m_dst);
    }
    if (hasWildcard(m_src))
    {
        WIN32_FIND_DATA find_data;
        HANDLE find_iter = FindFirstFile(m_src.c_str(), &find_data);
        if (find_iter == INVALID_HANDLE_VALUE)
        {
            _tprintf(_T("系统找不到指定的文件\n"));
        }
        do 
        {
            if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                continue;
            }
            MyString file_name(find_data.cFileName);
            if (hasWildcard(m_dst))
            {
                if (!match(file_name, basename(m_dst)))
                {
                    continue;
                }
                MyString src = join(dirname(m_src), file_name);
                MyString dst = join(dirname(m_dst), file_name);
                if (copyFile(src, dst, vfs) == -1)
                {
                    assert(0);
                    continue;
                }
            }
            else
            {
                MyString src = join(dirname(m_src), file_name);
                MyString dst = join(m_dst, file_name);
                if (!vfs->isDir(m_dst))
                {
                    _tprintf(_T("系统找不到指定的目录\n"));
                    continue;
                }
                if (copyFile(src, dst, vfs) == -1)
                {
                    assert(0);
                    continue;
                }
            }
        }
        while (FindNextFile(find_iter, &find_data) != 0);
    }
    else
    {
        if (vfs->isDir(m_dst))
        {
            m_dst = join(m_dst, basename(m_src));
        }
        else
        {
            MyString file_name = basename(m_dst);
            if (!isLegalName(file_name))
            {
                throw CommandException(_T("文件名不能包含非法字符\n"));
            }
        }
        copyFile(m_src, m_dst, vfs);
    }
    return;
}

int CommandCopy::copyFile(MyString src, MyString dst, VirtualDiskNode* vfs)
{
    HANDLE src_file = CreateFile(src.c_str(),
                                 GENERIC_READ,
                                 FILE_SHARE_READ,
                                 nullptr,
                                 OPEN_EXISTING,
                                 FILE_ATTRIBUTE_NORMAL,
                                 nullptr);
    if (src_file == INVALID_HANDLE_VALUE) 
    {
        throw CommandException(_T("系统找不到指定的文件\n"));
    }
    DWORD file_size = GetFileSize(src_file, nullptr);
    //CHAR *src_buf = new CHAR[file_size];
    DelegateMem<char> src_buf(new char[file_size]);
    DWORD read_bytes = 0;
    if (!ReadFile(src_file, 
                  src_buf,
                  file_size,
                  &read_bytes,
                  nullptr)
        || file_size != read_bytes)
    {
        //delete[] src_buf;
        CloseHandle(src_file);
        throw CommandException(src + _T("文件读取失败\n"));
    }

    FileHandler dst_file(nullptr);
    if (vfs->isFile(dst))
    {
        _tprintf(_T("覆盖文件%s,是否确认<Y/N>?"), basename(dst).c_str());
        TCHAR confirm = 0;
        _tscanf_s(_T("%c%*c"), &confirm, sizeof(confirm));
        if (confirm != 'y' && confirm != 'Y')
        {
            return 0;
        }
        dst_file = vfs->openFile(dst);
        if (!dst_file.isValid())
        {
            //delete[] src_buf;
            CloseHandle(src_file);
            throw CommandException(_T("无法打开/创建目标文件\n"));
        }
    }
    else
    {
        dst_file = vfs->createFile(dst);
        if (!dst_file.isValid())
        {
            CloseHandle(src_file);
            throw CommandException(_T("无法打开/创建目标文件\n"));
        }
    }
    // TODO : 文件写入
    dst_file.write(src_buf, file_size);
    //delete[] src_file;
    CloseHandle(src_file);
    return 0;
}