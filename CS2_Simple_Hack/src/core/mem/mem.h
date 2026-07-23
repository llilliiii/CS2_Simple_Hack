#pragma once
#include <cstdint>
#include <string>

namespace Mem {
	uintptr_t PatternScan(const std::string& pattern, const std::string& modStr);
}