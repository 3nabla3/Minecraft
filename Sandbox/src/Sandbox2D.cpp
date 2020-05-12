#include "Sandbox2D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
	:Layer("Sandbox 2D"), m_CameraController(1.778f, true)
{
}

void Sandbox2D::OnAttach()
{
	HZ_PROFILE_FUNCTION();
	m_Texture = Hazel::Texture2D::Create("assets/textures/DonkeyKong.png");
}

void Sandbox2D::OnDetach()
{
	HZ_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Hazel::TimeStep ts)
{
	m_CameraController.OnUpdate(ts);

	Hazel::Renderer2D::ResetStats();

	Hazel::RenderCommand::Clear();

	Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
	/*for (int x = 0; x < 1000; x++)
	{
		for (int y = 0; y < 100; y++)
		{
			Hazel::Renderer2D::DrawQuad({ x, y }, { 0.001f * x, 0.01f * y, 0.0f, 1.0f }, { 0.75f, 0.75f });
		}
	}*/

	Hazel::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.2f, 0.3f, 1.0f }, { 0.8f, 0.8f });
	Hazel::Renderer2D::DrawRotatedQuad({  0.8f, -0.5f }, 3.1415f / 4.0f, { 0.0f, 0.0f, 1.0f, 1.0f }, { 0.3f, 1.3f });
	Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f }, m_Texture, { 1.0f,1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, 10.0f);
	Hazel::Renderer2D::DrawRotatedQuad({ -0.5f, -0.5f }, 3.1415f, m_Texture, { 1.2f,1.2f }, { 1.0f, 1.0f, 1.0f, 1.0f }, 1.0f);
	Hazel::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	using namespace ImGui;

	auto stats = Hazel::Renderer2D::GetStats();
	Text("Renderer 2D Stats:");
	Text("Draw calls: %d", stats.DrawCalls);
	Text("Quad count: %d", stats.QuadCount);
	Text("Vertex count: %d", stats.GetTotalVertexCount());
	Text("Index count: %d", stats.GetTotalIndexCount());
}

void Sandbox2D::OnEvent(Hazel::Event& e)
{
	m_CameraController.OnEvent(e);
}