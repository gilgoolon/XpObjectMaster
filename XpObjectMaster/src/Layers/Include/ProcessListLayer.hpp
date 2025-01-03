#pragma once
#include <map>
#include <string>
#include <vector>

#include "UnopenedProcess.hpp"
#include "Walnut/Layer.h"

enum class ProcessListColumn : uint8_t
{
	PID = 0,
	FILENAME,
	COMMAND_LINE,
	UP_TIME,
};

using ProcessListColumns = std::vector<ProcessListColumn>;

const std::map<ProcessListColumn, std::string> PROCESS_LIST_COLUMN_TO_STRING{
	{ProcessListColumn::PID, "Process ID"},
	{ProcessListColumn::FILENAME, "Filename"},
	{ProcessListColumn::COMMAND_LINE, "Command Line"},
	{ProcessListColumn::UP_TIME, "Time Of Creation"},
};

class ProcessListLayer final : public Walnut::Layer
{
public:
	explicit ProcessListLayer();
	~ProcessListLayer() override = default;
	ProcessListLayer(const ProcessListLayer&) = delete;
	ProcessListLayer& operator=(const ProcessListLayer&) = delete;
	ProcessListLayer(ProcessListLayer&&) = delete;
	ProcessListLayer& operator=(ProcessListLayer&&) = delete;

	void OnUIRender() override;

	void OnUpdate(float ts) override;

	void update();

private:
	static void render_process(const UnopenedProcess& process, const ProcessListColumns& columns);
	static void render_process_column(const Process& process, ProcessListColumn column);

	std::vector<UnopenedProcess::Ptr> m_processes;
	ProcessListColumns m_columns;

	static constexpr Time::Duration UPDATE_COOLDOWN = Time::Seconds(1);
};
