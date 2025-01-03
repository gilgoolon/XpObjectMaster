#pragma once
#include <filesystem>
#include <Windows.h>

#include "Time.hpp"
#include "Process.hpp"
#include "WaitStatus.hpp"

namespace WinApi
{
[[nodiscard]] WaitStatus wait(HANDLE handle, Time::Duration timeout);

void load_library_to_process(Process& process, const std::filesystem::path& dll_path, Time::Duration timeout);
}
