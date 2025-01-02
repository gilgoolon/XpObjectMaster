#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Image.h"

#include "Function.hpp"

class ExampleLayer final : public Walnut::Layer
{
public:
	void OnUIRender() override
	{
		ImGui::Begin("Hello");
		ImGui::Button("Button");
		function();
		ImGui::End();

		ImGui::ShowDemoWindow();
	}
};

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	ApplicationSpecification spec;
	spec.Name = "Walnut Example";

	auto app = std::make_unique<Application>(spec);
	app->PushLayer<ExampleLayer>();
	app->SetMenubarCallback([&app]
	{
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Exit")) {
				app->Close();
			}
			ImGui::EndMenu();
		}
	});
	return app.release();
}
