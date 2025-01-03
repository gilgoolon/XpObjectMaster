#include "Include/UnopenedProcess.hpp"

UnopenedProcess::UnopenedProcess(const uint32_t pid):
	m_pid(pid)
{
}

Process::Ptr UnopenedProcess::open() const
{
	return std::make_unique<Process>(m_pid);
}
