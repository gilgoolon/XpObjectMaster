#pragma once
#include <memory>

#include "ScopedHandle.hpp"
#include "Time.hpp"

#include <string>
#include <vector>
#include <winternl.h>

class Process final
{
public:
	using Ptr = std::unique_ptr<Process>;

	explicit Process(uint32_t pid);
	~Process() = default;
	Process(const Process&) = delete;
	Process& operator=(const Process&) = delete;
	Process(Process&&) = delete;
	Process& operator=(Process&&) = delete;

private:
	[[nodiscard]] static HANDLE open_process(uint32_t pid);

	[[nodiscard]] PROCESS_BASIC_INFORMATION query_basic_information() const;

public:
	[[nodiscard]] uint32_t get_pid() const;

	[[nodiscard]] std::string get_filename() const;

	[[nodiscard]] bool is_running() const;

	[[nodiscard]] Time::Duration get_up_time() const;

	[[nodiscard]] std::string get_command_line() const;

	[[nodiscard]] std::vector<uint8_t> read_memory(const void* address, size_t size) const;

private:
	ScopedHandle m_handle;
};
