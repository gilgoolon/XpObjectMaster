#include "Include/ProcessListLayer.hpp"

#include "imgui.h"

#include <algorithm>
#include <functional>

ProcessListLayer::ProcessListLayer():
	Layer(),
	m_processes(),
	m_columns{ProcessListColumn::PID, ProcessListColumn::FILENAME, ProcessListColumn::UP_TIME}
{
}

void ProcessListLayer::OnUIRender()
{
	ImGui::Begin("Processes");

	if (ImGui::BeginTable(
		"Processes",
		static_cast<int>(m_columns.size()),
		ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg
	))
	{
		for (const ProcessListColumn column : m_columns)
		{
			ImGui::TableSetupColumn(PROCESS_LIST_COLUMN_TO_STRING.at(column).c_str());
		}
		ImGui::TableHeadersRow();
		for (const UnopenedProcess::Ptr& process : m_processes)
		{
			render_process(*process, m_columns);
		}
		ImGui::EndTable();
	}

	ImGui::End();
}

void ProcessListLayer::render_process(const UnopenedProcess& process, const ProcessListColumns& columns)
{
	ImGui::TableNextRow();
	const Process::Ptr opened_process = process.open();
	for (size_t i = 0; i < columns.size(); ++i)
	{
		ImGui::TableSetColumnIndex(static_cast<int>(i));
		render_process_column(*opened_process, columns.at(i));
	}
}

void ProcessListLayer::render_process_column(const Process& process, const ProcessListColumn column)
{
	std::string text;
	switch (column)
	{
	case ProcessListColumn::PID:
		text = std::to_string(process.get_pid());
		break;
	case ProcessListColumn::FILENAME:
		text = process.get_filename();
		break;
	case ProcessListColumn::UP_TIME:
		text = Time::to_string(process.get_up_time());
		break;
	case ProcessListColumn::COMMAND_LINE:
		text = process.get_command_line();
		break;
	}
	ImGui::Text("%s", text.c_str());
}
