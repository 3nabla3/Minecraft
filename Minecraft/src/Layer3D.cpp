#include "Layer3D.h"

Layer3D::Layer3D()
	:Layer("Layer 3D"), m_Controller(45.0f, 1.778f, { 0.0f, 3.0f, 0.0f })
{
	m_TextureDirt = UploadTexture("dirt");
	m_TextureSand = UploadTexture("sand");
	m_TextureTNT = UploadTexture("tnt");

	m_Skybox = Hazel::TextureCubeMap::Create({
		"assets/skybox/right.jpg",
		"assets/skybox/left.jpg",
		"assets/skybox/top.jpg",
		"assets/skybox/bottom.jpg",
		"assets/skybox/front.jpg",
		"assets/skybox/back.jpg"
		});

	m_Controller.m_CameraTranslationSpeed = 50.0f;
}



void Layer3D::OnUpdate(Hazel::TimeStep ts)
{
	m_Controller.OnUpdate(ts);

	Hazel::RenderCommand::SetClearColor({ 0.0f, 1.0f, 0.0f, 1.0f });
	Hazel::RenderCommand::Clear();
	Hazel::Renderer::ResetStats();

	Hazel::Renderer::BeginScene(m_Controller.GetCamera());
	for (uint32_t i = 0; i < 150; i++)
		for (uint32_t j = 0; j < 150; j++)
			Hazel::Renderer::DrawTexturedCube({ i * 2.0f, -5.0f, j * 2.0f }, m_TextureDirt, { 1.0f, 1.0f, 1.0f });
	Hazel::Renderer::EndScene();
	Hazel::Renderer::DrawSkybox(m_Skybox);
}

void Layer3D::OnEvent(Hazel::Event& e)
{
	m_Controller.OnEvent(e);
}

void Layer3D::OnImGuiRender()
{
	using namespace ImGui;
	auto stats = Hazel::Renderer::GetStats();

	Begin("Render stats");
	Text("Draw calls %d", stats.DrawCalls);
	Text("Cube count %d", stats.CubeCount);
	End();
}

Hazel::Ref<Hazel::TextureCubeMap> Layer3D::UploadTexture(const std::string& name)
{
	std::string rootDir = "assets/textures/";
	std::string ext = ".png";

	return Hazel::TextureCubeMap::Create({
		rootDir + name + "/side" + ext,
		rootDir + name + "/side" + ext,
		rootDir + name + "/top" + ext,
		rootDir + name + "/bottom" + ext,
		rootDir + name + "/side" + ext,
		rootDir + name + "/side" + ext,
		});
}
