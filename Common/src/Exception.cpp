﻿#include "Include/Exception.hpp"

#include <Windows.h>

Exception::Exception(const ErrorCode code):
	m_code(code)
{
}

ErrorCode Exception::code() const
{
	return m_code;
}

bool Exception::operator==(const ErrorCode error_code) const
{
	return m_code == error_code;
}

WinApiException::WinApiException(const ErrorCode code):
	Exception(code),
	m_error(GetLastError())
{
}

uint32_t WinApiException::error() const
{
	return m_error;
}

WinApiNtException::WinApiNtException(const ErrorCode code, const NTSTATUS status):
	Exception(code),
	m_status(status)
{
}

NTSTATUS WinApiNtException::status() const
{
	return m_status;
}
