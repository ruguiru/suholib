#include "FileReader.h"

using namespace suho;
using namespace suho::file;

FileReader::FileReader()
{
}

FileReader::FileReader(const std::string & filename)
{
    Open(filename);
}

FileReader::FileReader(const std::string & filename, const std::string & path)
{
    Open(filename, path);
}

FileReader::FileReader(const std::string & filename, const std::string & path, unsigned int openmode)
{
    Open(filename, path, openmode);
}

FileReader::~FileReader()
{
    Close();
}

bool FileReader::Open(const std::string & filename, const std::string & path, unsigned int openmode)
{
    if (_instream.is_open())
        _instream.close();

    std::string fullpath(path);
    fullpath += filename;

    _instream.open(fullpath, openmode);

    return _instream.is_open();
}

void FileReader::Close()
{
    if (_instream.is_open())
        _instream.close();
}

unsigned int FileReader::GetFileSize()
{
    if (!_instream.is_open())
        return -1;

    _instream.seekg(0, std::ios_base::beg);
    std::ifstream::pos_type begin_pos = _instream.tellg();
    _instream.seekg(0, std::ios_base::end);

    return static_cast<unsigned int>(_instream.tellg() - begin_pos);
}

void FileReader::ResetFilePointer()
{
    if (!_instream.is_open())
        return;

    _instream.seekg(0, std::ios::beg);
}

bool FileReader::Read(char * buffer, unsigned int buffersize)
{
    if (!_instream.is_open())
        return false;

    if (buffersize < GetFileSize())
        return false;

    ResetFilePointer();

    _instream.read(buffer, buffersize);

    return true;
}

bool FileReader::ReadLine(char* buffer, unsigned int buffersize)
{
    if (!_instream.is_open())
        return false;

    if (_instream.eof())
        return false;

    _instream.getline(buffer, buffersize);

    return true;
}

bool FileReader::ReadChar(char & ch)
{
    if (!_instream.is_open())
        return false;

    if (_instream.eof())
        return false;

    ch = _instream.get();

    return true;
}