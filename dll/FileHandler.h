#ifndef _FILE_HANDLER_H_
#define _FILE_HANDLER_H_

template<typename T>
class Vector;
class FileNode;
struct state;

class FileHandler
{
public:
    explicit FileHandler(FileNode* file);
    ~FileHandler(void);
    FileHandler(const FileHandler& handler);
    FileHandler& operator=(const FileHandler& handler);

    bool isValid(void);
    void close(void);
    state stat(void);
    Vector<char> read(void);
    void clear(void);
    void write(const char* buf, int len);
    void append(const char* buf, int len);
    void extend(int s);
private:
    FileNode* m_file;
};

#endif //_FILE_HANDLER_H_