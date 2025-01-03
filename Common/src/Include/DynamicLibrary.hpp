#pragma once
#include "Exception.hpp"

#include <filesystem>
#include <functional>
#include <Windows.h>

class DynamicLibrary final
{
public:
	explicit DynamicLibrary(const std::filesystem::path& dll_path);
	~DynamicLibrary();
	DynamicLibrary(const DynamicLibrary&) = delete;
	DynamicLibrary& operator=(const DynamicLibrary&) = delete;
	DynamicLibrary(DynamicLibrary&&) = delete;
	DynamicLibrary& operator=(DynamicLibrary&&) = delete;

private:
	[[nodiscard]] static HMODULE load_library(const std::filesystem::path& dll_path);

public:
	template <typename FunctionType, typename... Args>
	auto call(const std::string& name, Args&&... args) const
	{
		const FARPROC result = GetProcAddress(m_module, name.c_str());
		if (result == nullptr)
		{
			throw WinApiException(ErrorCode::FAILED_LIBRARY_GET_PROC_ADDRESS);
		}
		auto func = reinterpret_cast<FunctionType>(result);
		return (*func)(std::forward<Args>(args)...);
	}

private:
	HMODULE m_module;
};
