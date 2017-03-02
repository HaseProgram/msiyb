#pragma once

#include <stdio.h>
#include <memory>
#include <time.h>
#include <vector>

#ifdef _WIN32
#include <Windows.h>
#define PATH_MAX _MAX_PATH
#endif

#include "tools\exception.h"