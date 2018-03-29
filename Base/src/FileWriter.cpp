#include "FileWriter.h"

using namespace suho::file;

FileWriter::FileWriter()
{
    
}

FileWriter::FileWriter(const std::string & filename)
{
    Open(filename);
}

FileWriter::FileWriter(const std::string & filename, const std::string & path)
{
    Open(filename, path);
}

FileWriter::FileWriter(const std::string & filename, const std::string & path, unsigned int openmode)
{
    Open(filename, path, openmode);
}

FileWriter::~FileWriter()
{
    Close();
}

bool FileWriter::Open(const std::string & filename, const std::string & path, unsigned int openmode)
{
    if (_outstream.is_open())
        _outstream.close();

    std::string fullpath(path);
    fullpath += "\\";
    fullpath += filename;

    _outstream.open(fullpath, openmode);

    return _outstream.is_open();
}

void FileWriter::Close()
{
    if (_outstream.is_open())
        _outstream.close();
}