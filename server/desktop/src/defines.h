#pragma once

typedef unsigned char byte;
typedef unsigned long long size_lt;

#define MAX_INT 2147483646

#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#define PATH_MAX _MAX_PATH
#endif