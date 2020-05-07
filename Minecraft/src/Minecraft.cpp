#include <Hazel.h>
#include "Hazel/Core/EntryPoint.h"
#include "Layer3D.h"
#include "Layer2D.h"

class MinecraftApp : public Hazel::Application
{
public:
	MinecraftApp()
	{
		GetWindow().SetEventCallback(HZ_BIND_EVENT_FN(MinecraftApp::OnEvent));
		GetWindow().SetVSync(false);
		GetWindow().EnableCursor(false);

		PushLayer(new Layer3D());
		PushOverlay(new Layer2D());
	}

	void MinecraftApp::OnEvent(Hazel::Event& e)
	{
		Hazel::Application::OnEvent(e);
		Hazel::EventDispacher dispacher(e);
		dispacher.Dispach<Hazel::KeyPressedEvent>(HZ_BIND_EVENT_FN(MinecraftApp::OnKeyPressed));
	}

	bool MinecraftApp::OnKeyPressed(Hazel::KeyPressedEvent& e)
	{
		if (e.GetKeyCode() == HZ_KEY_ESCAPE && e.GetRepeatCount() == 0)
			GetWindow().EnableCursor(!GetWindow().IsCursorEnabled());

		return false;
	}
};

Hazel::Application* Hazel::CreateApplication()
{
	return new MinecraftApp();
}