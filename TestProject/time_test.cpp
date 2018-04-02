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
	std::string from_db_datetime("2012-02-28 00:00:20");						// DB ��¥�ð� �������� ������ ��Ʈ��

	DateTime dt;
	dt.SetDateTime(from_db_datetime, FORMAT(YYYY-MM-DD HH:MI:SS));				// DB�� ������ �������� ���
	printf("dt    : %s\n", dt.ToString(FORMAT(YYYYMMDDHHMISS)).c_str());		// ����ڰ� ������ �������� ���

	dt.Adjust(DAY(-10), SEC(-3), MIN(-1));										// ���õ� ��¥ ���� ����
	printf("dt    : %s\n", dt.ToString(FORMAT(YYYY-MM-DD HH:MI:SS)).c_str());	// �ٽ� ���˺����Ͽ� ���

	DateTime dt2;
	dt2.SetNow();																// ���� ��¥�ð����� ����
	printf("dt2   : %s\n", dt2.ToString(FORMAT(YYYYMMDD)).c_str());				// ����ڰ� ������ �������� ���(��¥�� ���)

	long long elapsed_seconds = dt2 - dt;										// �� ��¥�ð� ������ ���̸� �ʴ����� ����
	

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
	

	//StopWatch w;					// ������ ���� �������� ����
	w.Start();						// ���� �� Start()�� ����
	TimePoint t1;					// ����1
	this_thread::sleep_for(100ms);	
	TimeSpan ts = w.Stop();			// ����ð� ����

	TimePoint t2;					// ����2
	t2.Add(TimeSpan(30));			// ����2�� 30�� +

	ts = t2 - t1;					// ����2 - ����1 = ����ð�
	
	// ����ð��� �� ������ ��ȯ
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