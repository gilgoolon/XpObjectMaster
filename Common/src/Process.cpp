#include "Include/Process.hpp"

#include "Buffer.hpp"
#include "DynamicLibrary.hpp"
#include "Exception.hpp"
#include "Strings.hpp"
#include "WinApi.hpp"

#include <Psapi.h>
#include <Windows.h>
#include <winternl.h>

#pragma comment(lib, "ntdll.lib")

Process::Process(const uint32_t pid):
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

using NtQueryInformationProcessFunc = NTSTATUS(WINAPI*)(
	HANDLE process_handle,
	PROCESSINFOCLASS process_information_class,
	PVOID process_information,
	ULONG process_information_length,
	PULONG return_length);

PROCESS_BASIC_INFORMATION Process::query_basic_information() const
{
	const DynamicLibrary loaded_dll("ntdll.dll");
	PROCESS_BASIC_INFORMATION pbi;
	ULONG return_length = 0;
	const NTSTATUS result = loaded_dll.call<NtQueryInformationProcessFunc>(
		"NtQueryInformationProcess",
		m_handle.get(),
		ProcessBasicInformation,
		&pbi,
		static_cast<ULONG>(sizeof(pbi)),
		&return_length
	);
	if (!NT_SUCCESS(result))
	{
		throw WinApiNtException(ErrorCode::FAILED_PROCESS_QUERY_INFORMATION, result);
	}
	return pbi;
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
	const PROCESS_BASIC_INFORMATION basic_information = query_basic_information();

	const Buffer raw_peb = read_memory(basic_information.PebBaseAddress, sizeof(PEB));
	const auto* const peb = reinterpret_cast<const PEB*>(raw_peb.data());

	const Buffer raw_process_parameters = read_memory(peb->ProcessParameters, sizeof(RTL_USER_PROCESS_PARAMETERS));
	const auto* const process_parameters = reinterpret_cast<const RTL_USER_PROCESS_PARAMETERS*>(raw_process_parameters.
		data());

	const Buffer raw_command_line = read_memory(
		process_parameters->CommandLine.Buffer,
		process_parameters->CommandLine.Length
	);
	return Strings::to_string(Strings::to_wstring(raw_command_line));
}

std::vector<uint8_t> Process::read_memory(const void* const address, const size_t size) const
{
	std::vector<uint8_t> buffer(size);
	SIZE_T bytes_read = 0;
	const BOOL result = ReadProcessMemory(m_handle.get(), address, buffer.data(), size, &bytes_read);
	if (result == FALSE)
	{
		throw WinApiException(ErrorCode::FAILED_PROCESS_READ_MEMORY);
	}
	return buffer;
}
