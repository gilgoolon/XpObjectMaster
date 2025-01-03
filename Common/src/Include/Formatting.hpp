#pragma once
#include <string>
#include <sstream>

namespace Formatting
{
template <typename... Args>
std::wstring format(Args... args)
{
	std::wostringstream oss;
	((oss << args), ...);
	return oss.str();
}
}
