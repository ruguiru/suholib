#include "Log.h"

#ifdef  _WIN32
#include <Windows.h>
#include <shlobj.h>
#endif

using namespace suho::log;

Logger::Logger(const std::string & division, const std::string & path, cycle::FileCreateCycle cycle)
{
    Create(division, path);

    _path = path;
    _path += "\\";
    _path += division;

    _filecycle = cycle;
}

Logger::Logger(const Logger & other) :
    _path(other._path), _filecycle(other._filecycle)
{
}

Logger::Logger(Logger && other) : _path(std::move(other._path)), _filecycle(other._filecycle)
{
    other._path.clear();
}

Logger::~Logger()
{
}

void Logger::Create(const std::string & division, const std::string & path)
{
#ifdef _WIN32
#ifdef  UNICODE
    std::wstring wfullpath;
    wfullpath.assign(path.begin(), path.end());
    wfullpath += L"\\";
    wfullpath.append(division.begin(), division.end());
    SHCreateDirectoryEx(NULL, wfullpath.c_str(), NULL);

#else
    std::string fullpath(path);
    fullpath += "\\";
    fullpath += division;
    SHCreateDirectoryEx(NULL, fullpath.c_str(), NULL);
#endif //  UNICODE
#endif //  _WIN32
}

void Logger::CreateFolder(const std::string & date)
{
#ifdef _WIN32
#ifdef  UNICODE
    std::wstring wfullpath;
    wfullpath.assign(_path.begin(), _path.end());
    wfullpath += L"\\";
    wfullpath.append(date.begin(), date.end());
    SHCreateDirectoryEx(NULL, wfullpath.c_str(), NULL);
#else
    std::string fullpath(_path);
    fullpath += "\\";
    fullpath += date;
    SHCreateDirectoryEx(NULL, fullpath.c_str(), NULL);
#endif //  UNICODE
#endif //  _WIN32
}

std::string Logger::ConvertStringFromLoglevel(level::LogLevel loglevel)
{
    switch (loglevel)
    {
    case level::INFO:		    return "[INFO]";
    case level::FATAL:		    return "[FATAL]";
    case level::ERR:		    return "[ERROR]";
    case level::WARN:		    return "[WARN]";
    case level::DEBUG:		    return "[DEBUG]";
    default:		    break;
    }

    return "";
}