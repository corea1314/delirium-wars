#ifndef _FW_H
#define _FW_H

#ifdef WIN32
#include <windows.h>
#endif
#include "glee/GLee.h"	//todo: boot glee properly
#include "freeglut-2.4.0/include/gl/freeglut.h"
//#include <GL/glut.h>
#include <stdio.h>
#include <math.h>

#include "gfx.h"
#include "menu.h"

#include <algorithm>
#include <vector>
#include <map>

#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "macros.h"

#define SCREEN_SIZE_X	1280
#define SCREEN_SIZE_Y	 720

#endif//_FW_H