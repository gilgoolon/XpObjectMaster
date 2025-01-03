﻿#pragma once
#include <cstdint>

#include "ErrorCode.hpp"

#include <Windows.h>

class Exception
{
public:
	explicit Exception(ErrorCode code);
	virtual ~Exception() = default;
	Exception(const Exception&) = delete;
	Exception& operator=(const Exception&) = delete;
	Exception(Exception&&) = delete;
	Exception& operator=(Exception&&) = delete;

	[[nodiscard]] ErrorCode code() const;

private:
	ErrorCode m_code;
};

class WinApiException final : public Exception
{
public:
	explicit WinApiException(ErrorCode code);
	~WinApiException() override = default;
	WinApiException(const WinApiException&) = delete;
	WinApiException& operator=(const WinApiException&) = delete;
	WinApiException(WinApiException&&) = delete;
	WinApiException& operator=(WinApiException&&) = delete;

	[[nodiscard]] uint32_t error() const;

private:
	uint32_t m_error;
};

class WinApiNtException final : public Exception
{
public:
	explicit WinApiNtException(ErrorCode code, NTSTATUS status);
	~WinApiNtException() override = default;
	WinApiNtException(const WinApiNtException&) = delete;
	WinApiNtException& operator=(const WinApiNtException&) = delete;
	WinApiNtException(WinApiNtException&&) = delete;
	WinApiNtException& operator=(WinApiNtException&&) = delete;

	[[nodiscard]] NTSTATUS status() const;

private:
	NTSTATUS m_status;
};
