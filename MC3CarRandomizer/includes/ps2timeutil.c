#include "ps2timeutil.h"

// Convert sceCdCLOCK to Unix timestamp
//time_t sceCdCLOCK_to_unix(const sceCdCLOCK* clock) 
//{
//    struct tm t;
//
//    t.tm_sec = btoi(clock->second);
//    t.tm_min = btoi(clock->minute);
//    t.tm_hour = btoi(clock->hour);
//    t.tm_mday = btoi(clock->day);
//    t.tm_mon = btoi(clock->month) - 1;
//    t.tm_year = btoi(clock->year) + 100;
//
//    t.tm_isdst = -1;
//
//    return mktime(&t);
//}

unsigned int pack_sceCdCLOCK(const sceCdCLOCK* clock) 
{
    return (clock->year << 26) |
        (clock->month << 22) |
        (clock->day << 17) |
        (clock->hour << 12) |
        (clock->minute << 6) |
        (clock->second);
}
