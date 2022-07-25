#include "Vector3.h"

namespace CoolEngineName {
	template struct ENGINE_API Vector3<int8_t>;
	template struct ENGINE_API Vector3<uint8_t>;
	template struct ENGINE_API Vector3<int16_t>;
	template struct ENGINE_API Vector3<uint16_t>;
	template struct ENGINE_API Vector3<int32_t>;
	template struct ENGINE_API Vector3<uint32_t>;
	template struct ENGINE_API Vector3<long>;

	template struct ENGINE_API Vector3<float>;
	template struct ENGINE_API Vector3<double>;
}
