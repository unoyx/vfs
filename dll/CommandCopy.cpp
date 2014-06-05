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
        throw CommandException(_T("�����﷨����ȷ\n"));
    }
    m_src = pathes[0];
    if (!isAbs(m_src))
    {
        throw CommandException(_T("�����﷨����ȷ\n"));
    }
    m_dst = pathes[1];
    return;
}

void CommandCopy::setSwitches(const Vector<MyString>& switches)
{
    if (switches.size() != 0)
    {
        throw CommandException(_T("�����﷨����ȷ\n"));
    }
    return;
}

void CommandCopy::exec(VirtualDiskNode* vfs)
{
    if (!isNormalizedPath(m_dst))
    {
        m_dst = vfs->pathNormalize(m_dst);
    }
    
    if (!hasWildcard(m_src) && !m_src.endWith(_T("\\")))
    {
        DWORD attri = GetFileAttributes(m_src.c_str());
        if (attri & FILE_ATTRIBUTE_DIRECTORY)
        {
            m_src = join(m_src, _T("*"));
        }
    }

    if (hasWildcard(m_src))
    {
        WIN32_FIND_DATA find_data;
        HANDLE find_iter = FindFirstFile(m_src.c_str(), &find_data);
        if (find_iter == INVALID_HANDLE_VALUE)
        {
            throw CommandException(_T("ϵͳ�Ҳ���ָ�����ļ�\n"));
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
                    throw CommandException(_T("�ļ�:\"") + file_name + _T("\"����ʧ��\n"));
                }
                MyString src = join(dirname(m_src), file_name);
                MyString dst = join(dirname(m_dst), file_name);
                if (copyFile(src, dst, vfs) == -1)
                {
                    continue;
                }
            }
            else
            {
                MyString src = join(dirname(m_src), file_name);
                MyString dst = join(m_dst, file_name);
                if (!vfs->isDir(m_dst))
                {
                    throw CommandException(_T("ϵͳ�Ҳ���ָ����Ŀ¼\n"));
                }
                if (copyFile(src, dst, vfs) == -1)
                {
                    continue;
                }
            }
        }
        while (FindNextFile(find_iter, &find_data) != 0);
        FindClose(find_iter);
    }
    else
    {
        if (hasWildcard(m_dst))
        {
            MyString pattern = basename(m_dst);
            if (!match(basename(m_src), pattern))
            {
                return;
            }
            m_dst = dirname(m_dst);
        }
        if (vfs->isDir(m_dst))
        {
            m_dst = join(m_dst, basename(m_src));
        }
        else
        {
            MyString file_name = basename(m_dst);
            if (!isLegalName(file_name))
            {
                throw CommandException(_T("�ļ������ܰ����Ƿ��ַ�\n"));
            }
        }
        copyFile(m_src, m_dst, vfs);
    }
    return;
}

int CommandCopy::copyFile(MyString src, MyString dst, VirtualDiskNode* vfs)
{
    static const int BUF_SIZE = 1024 * 1024 * 10;
    HANDLE src_file = CreateFile(src.c_str(),
                                 GENERIC_READ,
                                 FILE_SHARE_READ,
                                 nullptr,
                                 OPEN_EXISTING,
                                 FILE_ATTRIBUTE_NORMAL,
                                 nullptr);
    if (src_file == INVALID_HANDLE_VALUE) 
    {
        throw CommandException(_T("ϵͳ�Ҳ���ָ�����ļ�\n"));
    }
    LARGE_INTEGER file_size;
    memset(&file_size, 0, sizeof(file_size));
    (void)GetFileSizeEx(src_file, &file_size);

    FileHandler dst_file(nullptr);
    if (vfs->isFile(dst))
    {
        _tprintf(_T("�����ļ�%s,�Ƿ�ȷ��<Y/N>?"), basename(dst).c_str());
        TCHAR confirm = 0;
        _tscanf_s(_T("%c%*c"), &confirm, sizeof(confirm));
        if (confirm != 'y' && confirm != 'Y')
        {
            throw CommandException(_T(""));
        }
        dst_file = vfs->openFile(dst);
        if (!dst_file.isValid())
        {
            CloseHandle(src_file);
            throw CommandException(_T("�޷���/����Ŀ���ļ�\n"));
        }
    }
    else
    {
        dst_file = vfs->createFile(dst);
        if (!dst_file.isValid())
        {
            CloseHandle(src_file);
            throw CommandException(_T("�޷���/����Ŀ���ļ�\n"));
        }

    }
    dst_file.clear();

    LARGE_INTEGER res_bytes = file_size;
    DelegateMem<char> src_buf(new char[BUF_SIZE]);
    DWORD read_bytes = 0;
    dst_file.extend(static_cast<int>(res_bytes.QuadPart));
    while (res_bytes.QuadPart > 0)
    {
        memset(src_buf, 0, BUF_SIZE);
        if (!ReadFile(src_file,
                      src_buf,
                      BUF_SIZE,
                      &read_bytes,
                      nullptr))
        {
            CloseHandle(src_file);
            throw CommandException(src + _T("�ļ���ȡʧ��\n"));
        }
        dst_file.append(src_buf, read_bytes);
        res_bytes.QuadPart -= read_bytes;
    }
    dst_file.close();
    CloseHandle(src_file);
    return 0;
}