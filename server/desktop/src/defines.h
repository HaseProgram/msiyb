#pragma once

typedef unsigned char byte;
//typedef unsigned long long size_lt;
typedef size_t size_lt;

#define MAX_INT 2147483646
#define MIN_BUFFER_SIZE 128
#define FILE_BUFFER_SIZE 1024
#define MIN_STRING_SIZE 128
#define MIN_STRING_ARRAY_SIZE 2
#define INFINITE            0xFFFFFFFF

#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#define PATH_MAX _MAX_PATH
#endif