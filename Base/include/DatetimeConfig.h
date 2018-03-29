#pragma once

namespace suho {
namespace datetime
{
    inline namespace date
    {
        enum MinMax {
            MINYEAR = 1900,
            MAXYEAR = 3000,

            MINMONTH = 1,
            MAXMONTH = 12,

            MINDAY = 1,
            MAXDAY = 31,            
        };

        enum DayofWeek {
            SUN = 0,
            MON,
            TUE,
            WED,
            THU,
            FRI,
            SAT,
        };
    }

    inline namespace time
    {
        enum MinMax {
            MINHOUR = 0,
            MAXHOUR = 23,

            MINMINUTE = 0,
            MAXMINUTE = 59,

            MINSECOND = 0,
            MAXSECOND = 59,

            MINTIME = 0,
            MAXTIME = 235959,
        };
    }
}   // end namespace datetime 
}   // end namespace suho