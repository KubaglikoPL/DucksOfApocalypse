#pragma once

#ifdef PSX_BUILD
#define PLATFORM_PSX
#else

#if defined(unix) || defined(__unix) || defined(__unix__) \
|| defined(_WIN32) || defined(__MACH__) || defined(__FreeBSD__) || defined(__EMSCRIPTEN__)
#define PLATFORM_DESKTOP
#endif

#endif // __linux__

#if defined(PLATFORM_DESKTOP)

#endif // defined(PLATFORM_DESKTOP)

#if defined(PLATFORM_PSX)

#endif

