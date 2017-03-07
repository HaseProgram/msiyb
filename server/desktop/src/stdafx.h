#pragma once

typedef unsigned char byte;
typedef unsigned long long size_lt;

#include <stdio.h>
#include <memory.h>
#include <time.h>

#include <vector>
#include <string>
#include "string.h"

#include <stdlib.h>
#include <string.h>
#include <iostream>


#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <process.h>
#define PATH_MAX _MAX_PATH
#endif

#include "tools\exception.h"
