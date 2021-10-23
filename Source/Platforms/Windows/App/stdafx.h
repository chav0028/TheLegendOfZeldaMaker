#ifndef GameDev2D_stdafx_h
#define GameDev2D_stdafx_h

//Windows precompiled header

#if _WIN32
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <WindowsX.h>
#include <Xinput.h>
#include <mmsystem.h>
#include <regstr.h>

#include "Shlwapi.h"
#pragma comment(lib, "shlwapi.lib")

#pragma comment(lib, "Xinput9_1_0")
#pragma comment(lib, "Winmm")

#endif

#include "glm.hpp"

#include <algorithm>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <sstream>
#include <vector>

#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <time.h>

#endif