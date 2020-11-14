/// ENIGMA Precompiled Header ///
#pragma once
#ifndef ENIGMA_PCH_H
#define ENIGMA_PCH_H


/// ENIGMA Core (Must be before platform includes to detect current platform)
#include <Enigma/Core/Core.hpp>
///


/// Platform specific includes ///
/// Platform Windows include
#if defined(ENIGMA_PLATFORM_WINDOWS)
#ifndef NOMINMAX
	// On Windows, a header file defines two macros min and max which may result in 
	// conflicts with their counterparts in the standard library and therefore in errors during compilation.
#define NOMINMAX
#endif
#include <Windows.h>
#endif
///


///ENIGMA Headers
#include "Enigma.hpp"
///

///OpenGL: GLAD
#include <glad/glad.h>
///

///Graphic library framwork: GLFW
#include <GLFW/glfw3.h>
///

///UI: ImGui
#include <imgui.h>
///


///Logger: SpdLog
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/basic_file_sink.h>
///


///pfd: portable file dialog
#include <portable-file-dialogs.h>
///


///STD 
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <chrono>
#include <random>
#include <algorithm>
#include <vector>
#include <array>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>
#include <stack>
#include <thread>
#include <memory>
#include <queue>
#include <deque>
#include <utility>
#include <list>
#include <future>
#include <functional>

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <cassert>
#include <cstdint>
///




#endif // !ENIGMA_PCH_H

