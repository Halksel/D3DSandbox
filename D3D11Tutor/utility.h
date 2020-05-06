#pragma once

#include <stdio.h>
#include <Windows.h>
#include <tchar.h>
#include <directxmath.h>

#define PI 3.14159265

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

