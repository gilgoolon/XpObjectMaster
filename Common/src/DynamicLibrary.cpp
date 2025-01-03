#include "Include/DynamicLibrary.hpp"

#include "Include/Exception.hpp"
#include "Include/Trace.hpp"

DynamicLibrary::DynamicLibrary(const std::filesystem::path& dll_path):
	m_module(load_library(dll_path))
{
}

DynamicLibrary::~DynamicLibrary()
{
	try
	{
		if (FreeLibrary(m_module) == FALSE)
		{
			TRACE(L"failed to free library")
		}
	}
	catch (...)
	{
		TRACE(L"failed to free library")
	}
}

HMODULE DynamicLibrary::load_library(const std::filesystem::path& dll_path)
{
	const HMODULE result = LoadLibraryW(dll_path.wstring().c_str());
	if (result == nullptr)
	{
		throw WinApiException(ErrorCode::FAILED_LIBRARY_LOAD);
	}
	return result;
}
