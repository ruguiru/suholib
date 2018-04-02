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
	std::string from_db_datetime("2012-02-28 00:00:20");						// DB 날짜시간 포맷으로 가져온 스트링

	DateTime dt;
	dt.SetDateTime(from_db_datetime, FORMAT(YYYY-MM-DD HH:MI:SS));				// DB와 동일한 포맷으로 등록
	printf("dt    : %s\n", dt.ToString(FORMAT(YYYYMMDDHHMISS)).c_str());		// 사용자가 지정한 포맷으로 출력

	dt.Adjust(DAY(-10), SEC(-3), MIN(-1));										// 세팅된 날짜 조정 연산
	printf("dt    : %s\n", dt.ToString(FORMAT(YYYY-MM-DD HH:MI:SS)).c_str());	// 다시 포맷변경하여 출력

	DateTime dt2;
	dt2.SetNow();																// 현재 날짜시간으로 세팅
	printf("dt2   : %s\n", dt2.ToString(FORMAT(YYYYMMDD)).c_str());				// 사용자가 지정한 포맷으로 출력(날짜만 출력)

	long long elapsed_seconds = dt2 - dt;										// 두 날짜시간 사이의 차이를 초단위로 리턴
	

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
	

	//StopWatch w;					// 생성시 현재 시작으로 세팅
	w.Start();						// 생성 후 Start()도 가능
	TimePoint t1;					// 시점1
	this_thread::sleep_for(100ms);	
	TimeSpan ts = w.Stop();			// 경과시간 리턴

	TimePoint t2;					// 시점2
	t2.Add(TimeSpan(30));			// 시점2에 30초 +

	ts = t2 - t1;					// 시점2 - 시점1 = 경과시간
	
	// 경과시간을 각 단위로 변환
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