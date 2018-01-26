#include "DateTime.h"

#include <chrono>

using namespace suho;
using namespace suho::datetime;

DateTime::DateTime()
{
    Init();

    _precalc_intermid_days = GetDaysFromFirstYear(MINYEAR, MINMONTH, MINDAY);
    _precalc_intermid_sec = _precalc_intermid_days * static_cast<long long>(86400);
}

DateTime::DateTime(const DateTime& other) :
	_year(other._year), _month(other._month), _day(other._day),
	_hour(other._hour), _min(other._min), _sec(other._sec)
{    
    _precalc_intermid_days = GetDaysFromFirstYear(MINYEAR, MINMONTH, MINDAY);
    _precalc_intermid_sec = _precalc_intermid_days * static_cast<long long>(86400);
}

DateTime::~DateTime()
{
}

void DateTime::Init()
{
	_year = MINYEAR;
	_month = MINMONTH;
	_day = MINDAY;
	_hour = MINHOUR;
	_min = MINMINUTE;
	_sec = MINSECOND;
}

DateTime& DateTime::operator=(const DateTime& other)
{
    if( this != &other )
    {
		_year = other._year;
		_month = other._month;
		_day = other._day;
		_hour = other._hour;
		_min = other._min;
		_sec = other._sec;
    }

    return *this;
}

bool DateTime::SetDateTime(int year, int month, int day, int hour, int minute, int second)
{
    if (year < MINYEAR || year > MAXYEAR)
        return false;

    if (month < MINMONTH || month > MAXMONTH)
        return false;

    if (day < MINDAY || day > MAXDAY)
        return false;

    if (hour < MINHOUR || hour > MAXHOUR)
        return false;

    if (minute < MINMINUTE || minute > MAXMINUTE)
        return false;

    if (second < MINSECOND || second > MAXSECOND)
        return false;

    _year = year;
    _month = month;
    _day = day;
    _hour = hour;
    _min = minute;
    _sec = second;

    return true;
}

bool DateTime::SetDateTime(long long datetime)
{
    char buf[128] = "";
    Itoa64(datetime, buf);

    std::string str(buf);

    if (str.size() < 14)
        return false;

    int year = static_cast<int>(atoi(str.substr(0, 4).c_str()));
    int month = static_cast<int>(atoi(str.substr(4, 2).c_str()));
    int day = static_cast<int>(atoi(str.substr(6, 2).c_str()));
    int hour = static_cast<int>(atoi(str.substr(8, 2).c_str()));
    int minute = static_cast<int>(atoi(str.substr(10, 2).c_str()));
    int second = static_cast<int>(atoi(str.substr(12, 2).c_str()));

    return SetDateTime(year, month, day, hour, minute, second);
}

bool DateTime::SetDate(int date)
{
    char buf[16] = "";
    _itoa_s(date, buf, 10);

    std::string str(buf);

    if (str.size() < 8)
        return false;

    int year = static_cast<int>(atoi(str.substr(0, 4).c_str()));
    int month = static_cast<int>(atoi(str.substr(4, 2).c_str()));
    int day = static_cast<int>(atoi(str.substr(6, 2).c_str()));

    return SetDate(year, month, day);
}

bool DateTime::SetDate(int year, int month, int day)
{
    if (year < MINYEAR || year > MAXYEAR)
        return false;

    if (month < MINMONTH || month > MAXMONTH)
        return false;

    if (day < MINDAY || day > MAXDAY)
        return false;

    _year = year;
    _month = month;
    _day = day;
    _hour = MINHOUR;
    _min = MINMINUTE;
    _sec = MINSECOND;

    return true;
}

void DateTime::SetNow()
{
	tm t;
    __time64_t long_time;

    _time64(&long_time);
    _localtime64_s(&t, &long_time);

	_year = t.tm_year + MINYEAR;
	_month = t.tm_mon + 1;
	_day = t.tm_mday;
	_hour = t.tm_hour;
	_min = t.tm_min;
	_sec = t.tm_sec;
}

int DateTime::GetAge() const
{
	DateTime now;
	now.SetNow();

	now.Adjust(YEAR(-_year), MONTH(-_month+1), DAY(-_day+1));

	return now.GetYear();
}

int DateTime::GetDayofWeek(int year, int month, int day)
{
    return GetDaysFromFirstYear(year, month, day) % 7;
}

int DateTime::GetDaysFromFirstYear(int year, int month, int day)
{
    // 그레고리안력
    int i;
    int mdays[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    long days_passed;

    /* 그리스도 탄생 원년이후 year의 전 년도 12월말 까지 경과된 날 수를 계산합니다.
    4, 100, 400을 나눈 것은 윤,평,년 일수를 보정하기 위한 것입니다. */
    days_passed = (long)(year - 1) * 365 + ((year - 1) / 4) - ((year - 1) / 100) + (((year - 1) / 400));

    /* 지정된 달의 이전월말까지 경과 날 수 */
    for (i = 0; i < month - 1; i++)
        days_passed += mdays[i];

    /* 지정된 달의 날자까지의 경과일 수 */
    days_passed += day;

    return days_passed;
}

bool DateTime::IsLeapYear(int year)
{
	if (year % 400 == 0)
		return true;
	if (year % 4 == 0 && year % 100 != 0)
		return true;

	return false;
}

long long DateTime::GetTotalSeconds() const
{
	return static_cast<long long>((GetDaysFromFirstYear(_year, _month, _day) * static_cast<long long>(86400)) + _hour * 3600 + _min * 60 + _sec);
}

char DateTime::Itoa64(const long long number, char* dest, int radix)
{
    if (number == 0)
        return 0;

    if (radix == 0)
        return 0;

	char rn = Itoa64(number / radix, dest);

	*(dest + rn) =  static_cast<char>(0x30 + (number % static_cast<long long>(radix)));

	return rn + 1;
}

void DateTime::AddYearImpl(const int y)
{
	_year += y;

	if (!IsLeapYear(_year) && _month == 2 && _day == 29)
		_day = 28;
}

void DateTime::AddMonthImpl(const int m)
{
	int sum_mon = _month + m;
	_month = sum_mon % 12;
	_year += sum_mon / 12;

	if (!IsLeapYear(_year) && _month == 2 && _day == 29)
		_day = 28;
}

void DateTime::AddDaysImpl(const int d)
{
	int remain_days = d;

	int mdays[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	while (true)
	{
		int md = mdays[_month - 1];
		if (IsLeapYear(_year) && _month == 2)
			++md;

		int cur_month_remainday = md - _day;

		if (remain_days <= cur_month_remainday)
		{
			break;
		}

		if (remain_days > cur_month_remainday)
		{
			remain_days -= cur_month_remainday;

			++_month;
			_day = 0;

			if (_month > 12)
			{
				_month = 1;
				++_year;
			}
		}
	}

	_day += remain_days;
}

void DateTime::AddSecondsImpl(const long long s)
{
	long long sum_sec = s + static_cast<long long>(_sec);
	_sec = sum_sec % 60;
	int carry_min = static_cast<int>(sum_sec / static_cast<long long>(60));

	if (carry_min <= 0)
		return;

	int sum_min = carry_min + _min;
	_min = sum_min % 60;
	int carry_hour = sum_min / 60;

	if (carry_hour <= 0)
		return;

	int sum_hour = carry_hour + _hour;
	_hour = sum_hour % 24;
	int day = sum_hour / 24;

	if (day <= 0)
		return;

	AddDaysImpl(day);
}

void DateTime::SubtractYearImpl(const int y)
{
	_year -= y;

	if (!IsLeapYear(_year) && _month == 2 && _day == 29)
		_day = 28;
}

void DateTime::SubtractMonthImpl(const int m)
{
	int sub_year = m / 12;
	int sub_month = m % 12;

	_year -= sub_year;
	if (_month > sub_month)
		_month -= sub_month;
	else
	{
		_month = (12 + _month) - sub_month;
		--_year;
	}		

	if (!IsLeapYear(_year) && _month == 2 && _day == 29)
		_day = 28;
}

void DateTime::SubtractSecondsImpl(const long long s)
{
    long long totsec = GetTotalSeconds();

    Init();

	AddSecondsImpl(totsec - _precalc_intermid_sec - s);
}

DateTime& suho::datetime::operator+(DateTime& dt, const YEAR & value)
{
	dt.AddYearImpl(value.Count());

	return dt;
}

DateTime& suho::datetime::operator+(DateTime& dt, const MONTH & value)
{
	dt.AddMonthImpl(value.Count());

	return dt;
}

DateTime& suho::datetime::operator+(DateTime& dt, const DAY & value)
{
	dt.AddDaysImpl(value.Count());

	return dt;
}

DateTime& suho::datetime::operator+(DateTime& dt, const HOUR & value)
{
	std::chrono::seconds s = std::chrono::duration_cast<SEC::DurationType>(value.duration);
	dt.AddSecondsImpl(s.count());
	return dt;
}

DateTime& suho::datetime::operator+(DateTime& dt, const MIN & value)
{
	std::chrono::seconds s = std::chrono::duration_cast<SEC::DurationType>(value.duration);
	dt.AddSecondsImpl(s.count());
	return dt;
}

DateTime& suho::datetime::operator+(DateTime& dt, const SEC & value)
{
	dt.AddSecondsImpl(value.Count());
	return dt;
}

DateTime& suho::datetime::operator-(DateTime& dt, const YEAR & value)
{
	dt.SubtractYearImpl(value.Count());
	return dt;
}

DateTime& suho::datetime::operator-(DateTime& dt, const MONTH & value)
{
	dt.SubtractMonthImpl(value.Count());
	return dt;
}

DateTime& suho::datetime::operator-(DateTime& dt, const DAY & value)
{
	std::chrono::seconds s = std::chrono::duration_cast<SEC::DurationType>(value.duration);
	dt.SubtractSecondsImpl(s.count());
	return dt;
}

DateTime& suho::datetime::operator-(DateTime& dt, const HOUR & value)
{
	std::chrono::seconds s = std::chrono::duration_cast<SEC::DurationType>(value.duration);
	dt.SubtractSecondsImpl(s.count());
	return dt;
}

DateTime& suho::datetime::operator-(DateTime& dt, const MIN & value)
{
	std::chrono::seconds s = std::chrono::duration_cast<SEC::DurationType>(value.duration);
	dt.SubtractSecondsImpl(s.count());
	return dt;
}

DateTime& suho::datetime::operator-(DateTime& dt, const SEC & value)
{
	dt.SubtractSecondsImpl(value.Count());
	return dt;
}

long long suho::datetime::operator-(const DateTime & advanced, const DateTime & lated)
{
    DateTime dt1(advanced);
    DateTime dt2(lated);

    return dt1.GetTotalSeconds() - dt2.GetTotalSeconds();
}
