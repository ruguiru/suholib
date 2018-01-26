#pragma once

#include <map>
#include <string>
#include <mutex>

#include "FileWriter.h"
#include "Singleton.h"

namespace suho {
namespace log
{
	namespace cycle
	{
		enum FileCreateCycle
		{
			DAILY,
			HOURLY,
		};
	}

	namespace level
	{
		enum LogLevel
		{
			INFO,
			FATAL,
			ERR,
			WARN,
			DEBUG,
		};
	}
    
    class Logger
    {
    public:
        Logger() {}
        Logger(const std::string& division, const std::string& path, cycle::FileCreateCycle cycle);
        ~Logger();

        Logger(const Logger& other);
        Logger(const Logger&& other);

        void Write(const std::string& message, level::LogLevel loglevel);

        suho::file::FileWriter& Write(level::LogLevel loglevel);

    private:
        std::string ConvertStringFromLoglevel(level::LogLevel loglevel);
        void CreateFolder(const std::string& date);

    private:
        suho::file::FileWriter							_fw;
		std::string										_path;
		cycle::FileCreateCycle							_filecycle;

        std::mutex										_mutex;
    };

    class LogSystem : public suho::pattern::singleton::Singleton<LogSystem>
    {
    public:
        LogSystem();
        ~LogSystem();

        void CreateLog(const std::string& division, const std::string& path, cycle::FileCreateCycle cycle);
        void WriteLog(const std::string& divison, const std::string& msg, level::LogLevel loglevel = level::INFO);

		suho::file::FileWriter& Write(const std::string& divison, level::LogLevel loglevel);

	private:
		Logger& GetLogger(const std::string& divison);

    private:
        std::map<std::string, Logger>          _log;
    };

#define BUILDLOG(division, path, cycle) suho::log::LogSystem::GetInstance()->CreateLog(division, path, cycle);
#define WRITELOG(division, msg, level) suho::log::LogSystem::GetInstance()->WriteLog(division, msg, level);

#define BEGINLOG(division, level) suho::log::LogSystem::GetInstance()->Write(division, level)
#define ENDLOG "\n" << suho::file::close
}   // end log
}   // end namespace suho