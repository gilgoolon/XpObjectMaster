#pragma once
#include "Buffer.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace Strings
{
[[nodiscard]] std::string to_string(const std::wstring& str);

[[nodiscard]] std::vector<uint8_t> to_buffer(const std::wstring& str);

[[nodiscard]] std::wstring to_wstring(const Buffer& buffer);
}
