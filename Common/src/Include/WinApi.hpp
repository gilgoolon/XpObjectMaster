#pragma once
#include "Time.hpp"
#include "WaitStatus.hpp"

namespace WinApi
{
[[nodiscard]] WaitStatus wait(HANDLE handle, Time::Duration timeout);
}
