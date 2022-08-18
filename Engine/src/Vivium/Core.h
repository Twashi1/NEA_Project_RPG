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
#include <limits>
#include <map>
#include <memory>
#include <numeric>
#include <queue>
#include <random>
#include <regex>
#include <set>
#include <string>
#include <thread>
#include <type_traits>
#include <unordered_map>
#include <vector>
#include <Windows.h>

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
	static entt::registry Registry;

	template <typename T> concept Arithmetic = std::is_arithmetic_v<T>;
	template <typename T> concept Signed = std::is_signed_v<T>;
	template <typename T> concept Integral = std::is_integral_v<T>;
	template <typename T> concept FloatingPoint = std::is_floating_point_v<T>;

	// TODO: move
	// Quake fast inverse square root
	// https://en.wikipedia.org/wiki/Fast_inverse_square_root
	VIVIUM_API float InverseSquareRoot(float number);
}
