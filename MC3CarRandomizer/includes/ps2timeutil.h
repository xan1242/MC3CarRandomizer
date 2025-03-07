#ifndef PS2TIMEUTIL_H
#define PS2TIMEUTIL_H

#include <libcdvd-common.h>
#include <time.h>


//time_t sceCdCLOCK_to_unix(const sceCdCLOCK* clock);
unsigned int pack_sceCdCLOCK(const sceCdCLOCK* clock);

#endif