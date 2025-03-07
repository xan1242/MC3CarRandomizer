#ifndef GPRINTF_H
#define GPRINTF_H

extern int(*gprintf)(const char* fmt, ...);
void gprintf_init();

#endif