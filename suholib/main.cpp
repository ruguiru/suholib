
#include <string>
#include <sys/timeb.h>
#include "DateTime.h"
#include "StopWatch.h"
#include "FileWriter.h"
#include "FileReader.h"
#include "UtilFunctions.h"
#include "Log.h"
#include "Timer.h"

#include <shlobj.h>

using namespace suho;
using namespace suho::datetime;
using namespace suho::time;
using namespace suho::file;
using namespace suho::idioms;
using namespace suho::log;

#ifdef UNICODE
#define STRING std::wstring
#else
#define STRING std::string
#endif

void foo(void* arg)
{    
    DateTime dt;
    dt.SetNow();

    printf("Flat : %s\n", dt.ToString(FORMAT(YYYY-MM-DD HH:MI:SS)).c_str());
}

void foo2(void* arg)
{
    DateTime dt;
    dt.SetNow();

    printf("Instant : %s\n", dt.ToString(FORMAT(YYYY-MM-DD HH:MI:SS)).c_str());
}

void foo3(void* arg)
{
    DateTime dt;
    dt.SetNow();

    printf("Normal : %s\n", dt.ToString(FORMAT(YYYY-MM-DD HH:MI:SS)).c_str());
}

int main()
{
    BUILDLOG("subfolder", "C:\\newfolder", cycle::HOURLY);
    //LOG("subfolder", "abcdefg", suho::log::DEBUG);
    //LOG("subfolder", "fjsflksj28934u98", suho::log::ERR);

    //suho::log::LogSystem::GetInstance()->Write("subfolder", suho::log::ERR) << "abcd" << 1234 << suho::file::close;

 //   BEGINLOG("subfolder", level::ERR) << "abcde " << 12134 << ENDLOG;

	WRITELOG("subfoler", "abdce", level::DEBUG);
 //   std::chrono::seconds dura(1);
 //   std::this_thread::sleep_for(dura);

    //Timer::GetInstance()->RegisterFlatTimer(foo, Timer::EVERY_5_SECOND);
    //Timer::GetInstance()->StartTimer();
    ////Timer t2;

    //Timer::GetInstance()->Join();
    //Timer t2;

    Timer t;
    t.RegisterFlatTimer(foo, Timer::EVERY_1_MINUTE);
    t.RegisterInstantTimer(foo2, 7);
    t.RegisterNormalTimer(foo3, 5);
    t.Begin();
    t.Join();

    

    //suho::log::LogSystem::GetInstance()->CreateLog("subfolder", "C:\\newfolder", suho::log::DAILY);
    //suho::log::LogSystem::GetInstance()->WriteLog("subfolder", "abcdefg", suho::log::DEBUG);

    //g_Dummy->Show();

    //DateTime dt;
    //dt.SetDateTime(20120228000020);

    //printf("dt    : %s\n", dt.ToString(FORMAT(YYYY-MM-DD HH:MI:SS)).c_str());
    //dt.Adjust(DAY(-10), SEC(-3), MIN(-1));
    //printf("dt    : %s\n", dt.ToString(FORMAT(YYYY-MM-DD HH:MI:SS)).c_str());

    //datetime::DateTime dt_t = dt - MONTH(3) - SEC(1) + YEAR(4) - MIN(1);
    //printf("dt_t  : %s\n", dt_t.ToString(FORMAT(YYYY-MM-DD HH:MI:SS)).c_str());


    //StopWatch w;

    //

    //TimeSpan span = w.Stop();

    //printf("min:%lf\n", span.ToMinutes());
    //printf("sec:%f\n", w.Stop().ToSeconds());
    //printf("millisec:%lld\n", w.Stop().ToMiliSeconds());
    //printf("microsec:%lld\n", w.Stop().ToMicroSeconds());
    //printf("nanosec:%lld\n", w.Stop().ToNanoSeconds());
    //

    //TimePoint t1;
    //TimePoint t2;

    //t2.Subtract(TimeSpan(30));

    //TimeSpan ts = t1 - t2;

    //printf("min:%lf\n", ts.ToMinutes());
    //printf("sec:%f\n", ts.ToSeconds());
    //printf("microsec:%lld\n", ts.ToMicroSeconds());
    //printf("nanosec:%lld\n", ts.ToNanoSeconds());

    //
    //FileWriter f("test.txt");
    //f << 123 << "\n";
    //f << "abc\n";
    //f << 1.232323f << "\n";
    //f.Close();

    //std::string b;
    //FileReader fr("test.txt");

    //if (fr.Eof())
    //{
    //    printf("eof\n");
    //}
 
    //int filesize = fr.GetFileSize();
    //printf("filesize : %d\n", filesize);

    //char* buf = new char[filesize];
    //memset(buf, 0, filesize);

    //fr.Read(buf, filesize);
    //delete[] buf;

    //FileReader fr2("test.txt");

    //while (fr2.Good())
    //{
    //    //char buf[1024] = {};
    //    //fr.ReadLine(buf, 1024);
    //    //printf("%s\n", buf);

    //    char ch;
    //    fr2.ReadChar(ch);
    //    printf("%c", ch);
    //}
}