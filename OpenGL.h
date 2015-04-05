#pragma once

#include <SFML/Config.hpp>

#if defined(SFML_SYSTEM_WINDOWS) || defined(SFML_SYSTEM_LINUX) || defined(SFML_SYSTEM_FREEBSD)
#ifdef _MSC_VER
#include <windows.h>
#endif
#include <GL/gl3.h>
#elif defined(SFML_SYSTEM_MACOS)
#include <OpenGL/gl3.h>
#endif

