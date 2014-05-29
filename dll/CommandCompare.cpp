#include "CommandCompare.h"
#include <Windows.h>
#include <stdio.h>
#include "path.h"
#include "DelegateMem.h"
#include "FileHandler.h"
#include "VirtualDiskNode.h"
#include "CommandException.h"

const int CommandCompare::OUTPUT_BYTE = 17;

CommandCompare::CommandCompare(void)
{
}

CommandCompare::~CommandCompare(void)
{
}

void CommandCompare::setPathes(const Vector<MyString>& pathes)
{
    if (pathes.size() != 2)
    {
        throw CommandException(_T("命令语法不正确\n"));
    }
    if (!isAbs(pathes[0]))
    {
        throw CommandException(_T("文件名、目录名或卷标语法不正确\n"));
    }
    m_src = pathes[0];
    m_dst = pathes[1];
    return;
}

void CommandCompare::setSwitches(const Vector<MyString>& switches)
{
    if (switches.size() > 0)
    {
        throw CommandException(_T("命令语法不正确\n"));
    }
    return;
}

void CommandCompare::exec(VirtualDiskNode* vfs)
{
    // destinate file
    if (!isNormalizedPath(m_dst))
    {
        m_dst = vfs->pathNormalize(m_dst);
    }
    FileHandler dst_file = vfs->openFile(m_dst);
    if (!dst_file.isValid())
    {
        throw CommandException(_T("系统找不到指定的文件\n"));
    }
    Vector<char> dst_buf = dst_file.read();

    // source file
    HANDLE src_file = CreateFile(m_src.c_str(),
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
//    char *src_buf = new char[file_size];
    DelegateMem<char> src_buf(new char[file_size]);
    DWORD read_bytes = 0;
    if (!ReadFile(src_file, 
                  src_buf,
                  file_size,
                  &read_bytes,
                  nullptr)
        || read_bytes != file_size)
    {
        // delete[] src_buf;
        CloseHandle(src_file);
        throw CommandException(m_src + _T("文件读取失败\n"));
    }

    bool isDifferent = false;
    int i = 0;
    for (; i < static_cast<int>(read_bytes) && i < dst_buf.size() && !isDifferent; ++i)
    {
        if (src_buf[i] != dst_buf[i])
        {
            isDifferent = true;
        }
    }
    if (!isDifferent) 
    {
        _tprintf(_T("内容完全一致\n"));
        //delete[] src_buf;
        //src_buf = nullptr;
        CloseHandle(src_file);
        return;
    }

    char output[OUTPUT_BYTE + 1] = {0};
    if (i < OUTPUT_BYTE)
    {
        for (int j = 0; j < i; ++j)
        {
            output[j] = dst_buf[j];
        }
        output[i] = '\0';
    }
    else
    {
        for (int j = (i - OUTPUT_BYTE); j < i; ++j)
        {
            output[j] = dst_buf[j];
        }
        output[OUTPUT_BYTE] = '\0';
    }
    if (m_src.endWith(_T(".txt")) || m_src.endWith(_T(".bat")))
    {
        printf("%s\n", output);
    }
    else
    {
        for (int j = 0; j < min(i, OUTPUT_BYTE); ++j)
        {
            printf("%X ", output[j]);
        }
        printf("\n");
    }
    //delete[] src_buf;
    //src_buf = nullptr;
    CloseHandle(src_file);
    return;
}
