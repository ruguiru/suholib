
#include <string>

#include "DateTime.h"

bool IsLeapYear(int year)
{
    if (year % 400 == 0)
        return true;
    if (year % 4 == 0 && year % 100 != 0)
        return true;

    return false;
}

void Add(const datetime::SEC& s)
{
    int year_ = 1985;
    int month_ = 10;
    int day_ = 21;
    int hour_ = 22;
    int min_ = 30;
    int sec_ = 50;

	sec_ = (s.Count() + static_cast<long long>(sec_)) % 60;

    int carry_min = s.Count() / 60;

	min_ = (carry_min + min_) % 60;

    int carry_hour = (carry_min + min_) / 60;

	hour_ = (carry_hour + hour_) % 24;

    int carry_day = (carry_hour + hour_) / 24;

    bool is_first_loop = true;
    while (carry_day >= 365)
    {       
        if (IsLeapYear(year_) )
        {
			//  첫번째 년도 2월 전으로 윤년 미적용된 경우 제외
			if (!is_first_loop)
				carry_day -= 366;
			else
			{
				if (month_ > 2)
					carry_day -= 365;
			}            
        }
		else
		{
			carry_day -= 365;
		}

		is_first_loop = false;

		++year_;
    }

	
    int mdays[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    int pivot_month = month_;
    while ( carry_day > 0 )
    {
		carry_day -= mdays[pivot_month];
		if (pivot_month == 2)
			carry_day -= 1;
    }
}

void Add(const datetime::DAY& d)
{
	int year_ = 2016;
	int month_ = 2;
	int day_ = 29;

	int day = d.Count();

	bool is_first_loop = true;
	while (day >= 365)
	{
		if (IsLeapYear(year_))
		{
			//  첫번째 년도 2월 전으로 윤년 미적용된 경우 제외
			if (!is_first_loop)
				day -= 366;
			else
			{
				if (month_ > 2)
					day -= 365;
			}
		}
		else
		{
			day -= 365;
		}

		is_first_loop = false;

		++year_;
	}


	int mdays[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	while (true)
	{
		int md = mdays[month_ - 1];
		if (IsLeapYear(year_) && month_ == 2)
			++md;

		// 이월
		if (day > md)
		{

			continue;
		}



		++month_;

		if (month_ > 12)
			month_ = 1;

		if (day < md - day_)
		{


			break;
		}		
	}

	day_ += day;

	printf("Add:%ddays   NewDate Year:%d Month:%d Day:%d\n", d.Count(), year_, month_, day_);
}

void SubTract()
{

}

int main()
{
    Add(datetime::DAY(1));


    datetime::DateTime dt;
    dt.SetDateTime("19701009180059", FORMAT(YYYYMMDDHHMISS));

	int var = 3;

	printf("dt   : %s\n", dt.ToString(FORMAT(YYYYMMDDHHMISS)).c_str());

	datetime::DateTime dt_t = dt + datetime::DAY(var) + datetime::DAY(4) + datetime::HOUR(300);

    printf("dt_t : %s\n", dt_t.ToString(FORMAT(YYYYMMDDHHMISS)).c_str());

    dt.Adjust(datetime::SEC(-10), datetime::YEAR(-1), var, datetime::MIN(25));

    printf("dt   : %s\n", dt.ToString(FORMAT(YYYYMMDDHHMISS)).c_str());

    tm t;
    t.tm_year = 70;
    t.tm_mon = 0;
    t.tm_mday = 1;
    t.tm_hour = 9;
    t.tm_min = 0;
    t.tm_sec = 0;
    t.tm_yday = 0;

    __time64_t tttt = _mktime64(&t);

    datetime::DateTime dt1;
    dt1.SetDateTime(19000101000000);

    datetime::DateTime dt2;
    dt2.SetDateTime(20201231235959);

    dt2 - dt1;



}