#include "test_common.h"
#include "LogHelper.h"

BUILD_LOGTYPE(TestLog, "C:/Log", cycle::FileCreateCycle::HOURLY)
#define TestLog TestLog::GetInstance()

int main()
{

    //userlog.Write(level::DEBUG, "%d %s %c", 1, "abc", 'A');

    TestLog->Write(level::LogLevel::DEBUG, "%d %s %c", 1, "abc", 'A');

	while (true)
	{
	}
}