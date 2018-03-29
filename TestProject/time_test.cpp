#include "test_common.h"
#include "DateTime.h"
#include "StopWatch.h"
#include "Timer.h"

using namespace std;
using namespace suho::datetime;
using namespace suho::time;


void foo(void*)
{
	DateTime dt;
	dt.SetNow();

	printf("Flat : %s\n", dt.ToString(FORMAT(YYYY-MM-DD HH:MI:SS)).c_str());
}

void foo2(void*)
{
	DateTime dt;
	dt.SetNow();

	printf("Instant : %s\n", dt.ToString(FORMAT(YYYY-MM-DD HH:MI:SS)).c_str());
}

void foo3(void*)
{
	DateTime dt;
	dt.SetNow();

	printf("Normal : %s\n", dt.ToString(FORMAT(YYYY-MM-DD HH:MI:SS)).c_str());
}

void main()
{
	DateTime dt;
	dt.SetDateTime(20120228000020);

	printf("dt    : %s\n", dt.ToString(FORMAT(YYYY-MM-DD HH:MI:SS)).c_str());
	dt.Adjust(DAY(-10), SEC(-3), MIN(-1));
	printf("dt    : %s\n", dt.ToString(FORMAT(YYYY-MM-DD HH:MI:SS)).c_str());

	DateTime dt_t = dt - MONTH(3) - SEC(1) + YEAR(4) - MIN(1);
	printf("dt_t  : %s\n", dt_t.ToString(FORMAT(YYYY-MM-DD HH:MI:SS)).c_str());

	StopWatch w;	
	this_thread::sleep_for(1s);
	TimeSpan span = w.Stop();

	printf("min:%lf\n", span.ToMinutes());
	printf("sec:%f\n", w.Stop().ToSeconds());
	printf("millisec:%lld\n", w.Stop().ToMiliSeconds());
	printf("microsec:%lld\n", w.Stop().ToMicroSeconds());
	printf("nanosec:%lld\n", w.Stop().ToNanoSeconds());
	

	TimePoint t1;
	this_thread::sleep_for(100ms);
	TimePoint t2;

	t2.Subtract(TimeSpan(30));

	TimeSpan ts = t2 - t1;

	printf("min:%lf\n", ts.ToMinutes());
	printf("sec:%f\n", ts.ToSeconds());
	printf("microsec:%lld\n", ts.ToMicroSeconds());
	printf("nanosec:%lld\n", ts.ToNanoSeconds());

	Timer::GetInstance()->RegisterFlatTimer(foo, Timer::EVERY_5_SECOND);
	Timer::GetInstance()->Start();
	Timer::GetInstance()->Join();

	Timer t;
	t.RegisterFlatTimer(foo, Timer::EVERY_1_MINUTE);
	t.RegisterInstantTimer(foo2, 7);
	t.RegisterNormalTimer(foo3, 5);
	t.Start();
	t.Join();
}