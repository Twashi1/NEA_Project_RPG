#pragma once

#include <algorithm>
#include <array>
#include <bitset>
#include <chrono>
#include <cmath>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <format>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <queue>
#include <regex>
#include <set>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <vector>

// Extern libraries
#include <entt.hpp>
#include <GL/glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

#define __VIVIUM_EXPOSE

#ifdef __VIVIUM_BUILD_DLL
	//#define __VIVIUM_EXPOSE
	#define VIVIUM_API __declspec(dllexport)
#else
	#define VIVIUM_API __declspec(dllimport)
#endif

#ifdef __VIVIUM_EXPOSE
	#define Ref(T) std::shared_ptr<T>
	#define MakeRef(T, ...) std::make_shared<T>(__VA_ARGS__)
#endif


namespace Vivium {
	// TODO: TMP!
	struct VIVIUM_API Color {
		union {
			struct { float x, y, z; };
			struct { float r, g, b; };
		};

		Color(float x, float y, float z);
	};
}