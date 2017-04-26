#pragma once

typedef unsigned char byte;
typedef unsigned long long size_lt;

#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#define PATH_MAX _MAX_PATH
#endif