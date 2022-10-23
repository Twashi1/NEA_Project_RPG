#pragma once

#include "Vector2.h"

namespace Vivium {
	class Serialiser;

	class VIVIUM_API Streamable {
	public:
		// TODO streamable constructor?
		virtual void Write(Serialiser& s) const = 0;
		virtual void Read(Serialiser& s) = 0;

		Streamable() = default;
		virtual ~Streamable() = default;
	};

	// For streaming types which only need their raw memory to be serialised/deserialised to be valid
	class VIVIUM_API StreamablePOD {
	public:
		StreamablePOD() = default;
		virtual ~StreamablePOD() = default;
	};

	// TODO still ugly
	template <typename T> inline constexpr bool __BaseStreamableTypes = std::is_arithmetic_v<T> || std::is_same_v<T, std::string> || std::is_same_v<T, bool> || std::is_base_of_v<StreamablePOD, T>;
	template <typename T> concept IsBaseStreamable = __BaseStreamableTypes<T>;
	template <typename T> concept IsStreamable = std::is_base_of_v<Streamable, T>
	|| __BaseStreamableTypes<T>
	|| std::is_same_v<T, Vector2<int8_t>>
	|| std::is_same_v<T, Vector2<uint8_t>>
	|| std::is_same_v<T, Vector2<int16_t>>
	|| std::is_same_v<T, Vector2<uint16_t>>
	|| std::is_same_v<T, Vector2<int32_t>>
	|| std::is_same_v<T, Vector2<uint32_t>>
	|| std::is_same_v<T, Vector2<long>>
	|| std::is_same_v<T, Vector2<float>>
	|| std::is_same_v<T, Vector2<double>>
	|| IsBaseStreamable<typename std::underlying_type<T>::type>;
}