#include "Include/Time.hpp"

#include "Exception.hpp"

#include <iomanip>
#include <sstream>

Time::Datetime Time::now()
{
	return std::chrono::system_clock::now();
}

Time::Datetime Time::to_datetime(const FILETIME& ft)
{
	static constexpr auto FILETIME_EPOCH_DIFFERENCE = Seconds(11644473600ULL);
	static constexpr uint64_t TICKS_PER_SECOND = 10'000'000;
	static constexpr uint64_t TICKS_TO_NANOS = 100;

	ULARGE_INTEGER ull{};
	ull.LowPart = ft.dwLowDateTime;
	ull.HighPart = ft.dwHighDateTime;

	const uint64_t total_seconds = ull.QuadPart / TICKS_PER_SECOND;
	const uint64_t total_nanoseconds = (ull.QuadPart % TICKS_PER_SECOND) * TICKS_TO_NANOS;
	return Datetime(
		duration_cast<std::chrono::system_clock::duration>(
			Seconds(total_seconds) + Nanos(total_nanoseconds) - FILETIME_EPOCH_DIFFERENCE
		)
	);
}

std::string Time::to_string(const Datetime& dt)
{
	const time_t time_t = std::chrono::system_clock::to_time_t(dt);
	std::tm tm = {};
	if (const errno_t result = localtime_s(&tm, &time_t); !result)
	{
		throw WinApiException(ErrorCode::FAILED_TIME_FORMAT_CONVERSION);
	}

	std::ostringstream oss;
	oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
	return oss.str();
}

std::string Time::to_string(const Duration& duration)
{
	std::ostringstream output;
	if (const auto days = duration_cast<Days>(duration); days.count() > 0)
	{
		output << days.count() << " days ";
	}
	if (const auto hours = duration_cast<Hours>(duration); hours.count() > 0)
	{
		output << hours.count() << " hrs ";
	}
	if (const auto minutes = duration_cast<Minutes>(duration); minutes.count() > 0)
	{
		output << minutes.count() << " mins ";
	}
	if (const auto seconds = duration_cast<Seconds>(duration); seconds.count() > 0)
	{
		output << seconds.count() << " secs ";
	}
	const std::string result = output.str();
	return result.substr(0, result.size() - 1);
}
