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

WinApiException::WinApiException(const ErrorCode code):
	Exception(code),
	m_error(GetLastError())
{
}

uint32_t WinApiException::error() const
{
	return m_error;
}