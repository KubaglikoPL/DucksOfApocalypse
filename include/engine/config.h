#pragma once

#ifdef PSX_BUILD
#define PLATFORM_PSX
#endif

#if defined(unix) || defined(__unix) || defined(__unix__) \
|| defined(_WIN32) || defined(__MACH__) || defined(__FreeBSD__)
#define PLATFORM_DESKTOP
#define GLFW_LIB
#endif

//#if defined(__EMSCRIPTEN__)
//#define GLFW_LIB
//#endif // defined(__EMSCRIPTEN__)

#if defined(PLATFORM_DESKTOP)
#define RGB8_SCREEN_BUFFER
#endif // defined(PLATFORM_DESKTOP)

#if defined(PLATFORM_PSX)

#endif

