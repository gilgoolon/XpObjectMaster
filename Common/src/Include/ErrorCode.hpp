﻿#pragma once
#include <cstdint>

enum class ErrorCode : uint32_t
{
	UNKNOWN_ERROR = 0,
	FAILED_TIME_FORMAT_CONVERSION,
	FAILED_PROCESS_GET_PID,
	FAILED_PROCESS_GET_FILENAME,
	FAILED_PROCESS_GET_TIMES,
	FAILED_WAIT,
	FAILED_PROCESS_OPEN,
};