#pragma once

#include <stdio.h>
#include <Windows.h>
#include <tchar.h>
#include <directxmath.h>

#define PI 3.14159265
#define Deg2Rad 0.01745329251
#define Rad2Deg 57.2957795131

#ifdef _DEBUG
#   define MyOutputDebugString( str, ... ) \
      { \
        TCHAR c[256]; \
        _stprintf_s( c, str, __VA_ARGS__ ); \
        OutputDebugString( c ); \
      }
#else
#    define MyOutputDebugString( str, ... ) // ‹óŽÀ‘•
#endif

