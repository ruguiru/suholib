
#ifdef  _WIN32
#include <Windows.h>
#include <shlobj.h>
#endif

#include "Log.h"
#include "DateTime.h"

using namespace suho;
using namespace suho::log;
using namespace suho::datetime;
using namespace suho::file;

Logger::Logger(const std::string& division, const std::string& path, cycle::FileCreateCycle cycle)
{
    _fw.Close();

    _path = path;
    _path += "\\";
    _path += division;

    _filecycle = cycle;
}

Logger::~Logger()
{
    _fw.Close();
}

Logger::Logger(const Logger & other)
    : _path(other._path), _filecycle(other._filecycle)
{
}

Logger::Logger(const Logger && other)
    :_path(std::move(other._path)), _filecycle(other._filecycle)
{
}

void Logger::Write(const std::string & message, level::LogLevel loglevel)
{   
    std::lock_guard<std::mutex> lock(_mutex);

    DateTime dt;
    dt.SetNow();

    std::string date = dt.ToString(FORMAT(YYYY-MM-DD HH:MI:SS)).substr(0, 10);
    std::string time = dt.ToString(FORMAT(YYYY-MM-DD HH:MI:SS)).substr(11, 8);

    if (_filecycle == cycle::DAILY)
    {
        std::string filename(date);
        filename += ".log";

        FileWriter fw(filename, _path, std::ios::app);
        fw << time << " " << ConvertStringFromLoglevel(loglevel) << " " << message <<"\n";        
    }
    else if (_filecycle == cycle::HOURLY)
    {       
        std::string filename = time.substr(0, 2);
        filename += "h.log";

        CreateFolder(date);

        std::string appended_path(_path);
        appended_path += "\\";
        appended_path += date;

        FileWriter fw(filename, appended_path, std::ios::app);
        fw << time << " " << ConvertStringFromLoglevel(loglevel) << " " << message << "\n";
    }
}

suho::file::FileWriter& Logger::Write(level::LogLevel loglevel)
{
    std::lock_guard<std::mutex> lock(_mutex);

    DateTime dt;
    dt.SetNow();

    std::string date = dt.ToString(FORMAT(YYYY-MM-DD HH:MI:SS)).substr(0, 10);
    std::string time = dt.ToString(FORMAT(YYYY-MM-DD HH:MI:SS)).substr(11, 8);

    _fw.Close();

    if (_filecycle == cycle::DAILY)
    {
        std::string filename(date);
        filename += ".log";

        _fw.Open(filename, _path, std::ios::app);
        _fw << time << " " << ConvertStringFromLoglevel(loglevel) << " ";
    }
    else if (_filecycle == cycle::HOURLY)
    {
        std::string filename = time.substr(0, 2);
        filename += "h.log";

        CreateFolder(date);

        std::string appended_path(_path);
        appended_path += "\\";
        appended_path += date;

        _fw.Open(filename, appended_path, std::ios::app);
        _fw << time << " " << ConvertStringFromLoglevel(loglevel) << " ";
    }

    return _fw;
}

std::string Logger::ConvertStringFromLoglevel(level::LogLevel loglevel)
{
    switch (loglevel)
    {
	case level::INFO:
        return "[INFO]";
    case level::FATAL:
        return "[FATAL]";
    case level::ERR:
        return "[ERROR]";
    case level::WARN:
        return "[WARN]";
    case level::DEBUG:
        return "[DEBUG]";
    default:
        break;
    }

    return "[INFO]";
}

void Logger::CreateFolder(const std::string& date)
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
    // 그 외 운영체제


#endif //  _WIN32
}


LogSystem::LogSystem()
{

}

LogSystem::~LogSystem()
{

}

void LogSystem::CreateLog(const std::string & division, const std::string & path, cycle::FileCreateCycle cycle)
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
    // 그 외 운영체제


#endif //  _WIN32

    _log.insert(std::make_pair(division, Logger(division, path, cycle)));        
}

void LogSystem::WriteLog(const std::string & divison, const std::string & msg, level::LogLevel loglevel)
{
    auto pos = _log.find(divison);
    if (pos != _log.end())
    {
        pos->second.Write(msg, loglevel);
    }
}

suho::file::FileWriter & LogSystem::Write(const std::string & divison, level::LogLevel loglevel)
{
	return GetLogger(divison).Write(loglevel);
}

Logger & LogSystem::GetLogger(const std::string & divison)
{
	auto pos = _log.find(divison);
	if (pos == _log.end())
	{
		CreateLog(divison, "\\.", cycle::DAILY);        // 잘못된 값이 들어온 경우 default 값
	}

	return _log[divison];
}

