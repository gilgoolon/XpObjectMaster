#pragma once
#include <memory>

#include "ScopedHandle.hpp"
#include "Time.hpp"

#include <string>

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

public:
	[[nodiscard]] uint32_t get_pid() const;

	[[nodiscard]] std::string get_filename() const;

	[[nodiscard]] bool is_running() const;

	[[nodiscard]] Time::Duration get_up_time() const;

	[[nodiscard]] std::string get_command_line() const;

private:
	ScopedHandle m_handle;
};
