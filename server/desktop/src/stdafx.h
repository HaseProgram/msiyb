#pragma once

#include <stdio.h>
#include <memory.h>
#include <time.h>

#include <vector>

#include <stdlib.h>
#include <string.h>
#include <iostream>


#ifdef _WIN32
#include <windows.h>
#include <process.h>
#define PATH_MAX _MAX_PATH
#endif

#include "tools\exception.h"