#pragma once

#include <fstream>

#include "NonCopyable.h"

namespace suho {
namespace file
{
    class FileReader : public suho::idioms::NonCopyable
    {
    public:
        FileReader();
        FileReader(const std::string& filename);
        FileReader(const std::string& filename, const std::string& path);
        FileReader(const std::string& filename, const std::string& path, unsigned int openmode);
        ~FileReader();

        bool Open(const std::string& filename, const std::string& path = ".\\", unsigned int openmode = std::ios::in);
        void Close();

        int GetFileSize();
        bool Eof() const { return _instream.eof(); }
        bool Good() { return _instream.good(); }
        void ResetFilePointer();

        // ���۷� �б�
        bool Read(char* buffer, int buffersize);       // ȣ��� ���������͸� ���������� �̵�
        bool ReadLine(char* buffer, int buffersize);
        bool ReadChar(char& ch);

        // Ÿ������ �б� (���๮�ڳ� ���鹮�� ������ ��� �д´�)
        template<typename T> inline FileReader& operator >> (T& data);

    private:
        std::ifstream           _instream;
    };

    template<typename T>
    inline FileReader & FileReader::operator >> (T & data)
    {
        _instream >> data;

        return *this;
    }
}   // end file
}   // end namespace suho