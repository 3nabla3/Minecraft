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
	m_Texture = Hazel::Texture2D::Create("assets/textures/checkerboard.png");
}

void Sandbox2D::OnDetach()
{
	HZ_PROFILE_FUNCTION();
}

void Sandbox2D::OnUpdate(Hazel::TimeStep ts)
{
	m_CameraController.OnUpdate(ts);
	
	Hazel::RenderCommand::Clear();

	Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
	Hazel::Renderer2D::DrawQuad(m_TexturePosition, m_Texture, m_TextureSize, m_TextureColor, 10.0f);
	Hazel::Renderer2D::DrawRotatedQuad(m_SquarePosition, 3.141f / 4, m_SquareColor, m_SquareSize);
	Hazel::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
}

void Sandbox2D::OnEvent(Hazel::Event& e)
{
	m_CameraController.OnEvent(e);
}