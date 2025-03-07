#include <ctype.h>
#include "stringhash.h"

// void* (*_strhash_malloc)(size_t size) = (void* (*)(size_t))(0);
// void* (*_strhash_free)(void* ptr) = (void* (*)(void*))(0);
// size_t(*_strhash_strlen)(const char* str) = (size_t(*)(const char*))(0);
// char* (*_strhash_strcpy)(char* dst, const char* src) = (char* (*)(char*, const char*))(0);

// inline void strToUpper(char* str) 
// {
//     while (*str) 
// 	{
//         *str = toupper(*str);
//         str++;
//     }
// }


int bStringHash(char* str)
{
	if (str == NULL) return 0;

	char* _str = ((char*)str);
	int   result = -1;

	while (*_str)
	{
		result = result * 0x21 + (unsigned int)(*_str);
		_str++;
	}

	return result;
}

// int bStringHashUpper(char* str)
// {
// 	char* hashstr = _strhash_malloc(_strhash_strlen(str) + 1);
// 	_strhash_strcpy(hashstr, str);
// 	strToUpper(hashstr);
// 	int result = bStringHash(hashstr);
// 	_strhash_free(hashtr);
// 	return result;
// }

// void strhash_SetMallocFunc(void* ptr)
// {
// 	_strhash_malloc = (void* (*)(size_t))(ptr);
// }
// 
// void strhash_SetFreeFunc(void* ptr)
// {
// 	_strhash_free = (void* (*)(void*))(ptr);
// }
// 
// void strhash_SetStrlenFunc(void* ptr)
// {
// 	_strhash_strlen = (size_t(*)(const char*))(ptr);
// }
// 
// void strhash_SetStrcpyFunc(void* ptr)
// {
// 	_strhash_strcpy = (char* (*)(char*, const char*))(ptr);
// }
