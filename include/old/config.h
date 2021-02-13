#pragma once

#ifdef PSX_BUILD
#define PLATFORM_PSX
#else

#if defined(unix) || defined(__unix) || defined(__unix__) \
|| defined(_WIN32) || defined(__MACH__) || defined(__FreeBSD__) || defined(__EMSCRIPTEN__)
#define PLATFORM_DESKTOP
#endif

#endif // __linux__

//#define PACKED_GAME_DATA

#if defined(PLATFORM_DESKTOP)
#define PACKED_GAME_DATA
#define OFFSCREEN_BUFFER_RENDER
#define HAVE_EXIT_OPTION
#define SUPPORT_MAP_SAVING
#endif // defined(PLATFORM_DESKTOP)

#if defined(PLATFORM_PSX)
#define NO_HARD_DRIVE_DATA
#define REVERSED_DRAW_ORDER
#define CONTINUES_INPUT
#endif

