#pragma once


#include <iostream>
#include <map>
#include <string>
#include <mutex>

#include "FileWriter.h"
#include "DateTime.h"
#include "Singleton.hpp"

namespace suho {
namespace log
{
	using namespace suho::datetime;
	using namespace suho::file;
	using namespace std;

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
            NONE,
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
        Logger(const Logger& other);
        Logger(Logger&& other);
        virtual ~Logger();

		template<typename ... Args>
        inline void Write(level::LogLevel loglevel, const std::string & fmt, const Args&... args);

    private:
		void Create(const std::string & division, const std::string & path);
        void CreateFolder(const std::string& date);

		std::string ConvertStringFromLoglevel(level::LogLevel loglevel);

    private:
		FileWriter										_fw;
		std::string										_path;
		cycle::FileCreateCycle							_filecycle;

        std::mutex										_mutex;
    };

	template<typename ...Args>
	inline void Logger::Write(level::LogLevel loglevel, const std::string& fmt, const Args&... args)
	{       
		std::lock_guard<std::mutex> lock(_mutex);

		size_t size = snprintf(nullptr, 0, fmt.c_str(), args...) + 1;
		std::unique_ptr<char[]> buf(new char[size]);
		snprintf(buf.get(), size, fmt.c_str(), args ...);
		std::string message(buf.get(), buf.get() + size - 1);

		DateTime dt;
		dt.SetNow();		

		std::string date = dt.ToString(FORMAT(YYYY-MM-DD HH:MI:SS)).substr(0, 10);
		std::string time = dt.ToString(FORMAT(YYYY-MM-DD HH:MI:SS)).substr(11, 8);		

		if (_filecycle == cycle::DAILY)
		{
			std::string filename(date);
			filename += ".log";

			_fw.Open(filename, _path, std::ios::app);            
			_fw << time << " " << ConvertStringFromLoglevel(loglevel) << " " << message << "\n" << close;
//#ifdef _DEBUG
			cout << time << " " << ConvertStringFromLoglevel(loglevel) << " " << message << "\n";
//#endif
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
			_fw << time << " " << ConvertStringFromLoglevel(loglevel) << " " << message << "\n" << close;
//#ifdef _DEBUG
			cout << time << " " << ConvertStringFromLoglevel(loglevel) << " " << message << "\n";
//#endif
		}
	}


}   // end log
}   // end namespace suho


