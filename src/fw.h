#ifndef _FW_H
#define _FW_H

#ifdef WIN32
#include <windows.h>
#endif
#include "freeglut-2.4.0/include/gl/freeglut.h"
//#include <GL/glut.h>
#include <stdio.h>
#include <math.h>

#ifndef WIN32
#include <GL/glx.h>
#endif

#include "gfx.h"
#include "menu.h"

#include <algorithm>
#include <vector>
#include <map>

#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "macros.h"

#endif//_FW_H