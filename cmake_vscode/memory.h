#pragma once

#include <gba_base.h>
#include <stdint.h>

namespace Memory
{

	/// @brief Copy dwords from source to destination.
	/// @param destination Copy destination.
	/// @param source Copy source.
	/// @param count Number of dwords to copy.
	extern "C" void memcpy32(void *destination, const void *source, uint32_t count) IWRAM_CODE;

}