#include "Include/Process.hpp"

#include "Exception.hpp"
#include "WinApi.hpp"

#include <Psapi.h>
#include <Windows.h>

Process::Process(uint32_t pid):
	m_handle(open_process(pid))
{
}

HANDLE Process::open_process(const uint32_t pid)
{
	static constexpr DWORD ALL_ACCESS = PROCESS_ALL_ACCESS;
	static constexpr BOOL DONT_INHERIT_HANDLE = FALSE;
	const HANDLE result = OpenProcess(ALL_ACCESS, DONT_INHERIT_HANDLE, pid);
	if (result == INVALID_HANDLE_VALUE)
	{
		throw WinApiException(ErrorCode::FAILED_PROCESS_OPEN);
	}
	return result;
}

uint32_t Process::get_pid() const
{
	const uint32_t result = GetProcessId(m_handle.get());
	if (result == 0)
	{
		throw WinApiException(ErrorCode::FAILED_PROCESS_GET_PID);
	}
	return result;
}

std::string Process::get_filename() const
{
	static constexpr char NULL_TERMINATOR = '\0';
	std::string filename(MAX_PATH, NULL_TERMINATOR);
	const DWORD result = GetProcessImageFileNameA(m_handle.get(), filename.data(), static_cast<DWORD>(filename.size()));
	if (result == 0)
	{
		throw WinApiException(ErrorCode::FAILED_PROCESS_GET_FILENAME);
	}
	filename.resize(result);
	return filename;
}

bool Process::is_running() const
{
	const WinApi::WaitStatus result = WinApi::wait(m_handle.get(), Time::INSTANT);
	switch (result)
	{
	case WinApi::WaitStatus::FINISHED:
		[[fallthrough]];
	case WinApi::WaitStatus::OBJECT_CLOSED:
		return true;

	case WinApi::WaitStatus::TIMEOUT:
		return false;

	case WinApi::WaitStatus::FAILED:
		[[fallthrough]];
	default:
		throw WinApiException(ErrorCode::FAILED_WAIT);
	}
}

Time::Duration Process::get_up_time() const
{
	FILETIME creation_time{};
	FILETIME exit_time{};
	FILETIME kernel_time{};
	FILETIME user_time{};
	if (GetProcessTimes(
		m_handle.get(),
		&creation_time,
		&exit_time,
		&kernel_time,
		&user_time
	) == FALSE)
	{
		throw WinApiException(ErrorCode::FAILED_PROCESS_GET_TIMES);
	}
	return Time::duration_cast<Time::Duration>(Time::to_datetime(creation_time) - Time::now());
}

std::string Process::get_command_line() const
{
	return "";
}
