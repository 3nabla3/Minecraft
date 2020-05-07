#include "Layer3D.h"

Layer3D::Layer3D()
	:Layer("Layer 3D"), m_Controller(45.0f, 1.778f, { 0.0f, 3.0f, 0.0f })
{
	m_TextureGrass = UploadTexture("dirt");
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
}

void Layer3D::OnUpdate(Hazel::TimeStep ts)
{
	m_Controller.OnUpdate(ts);

	Hazel::RenderCommand::Clear();

	Hazel::Renderer::BeginScene(m_Controller.GetCamera());
	for (int x = 0; x < 3; x++)
		for (int z = 0; z < 3; z++)
		{
			Hazel::Renderer::DrawTexturedCube({ -x * 2.0f , 0.0f, -z * 2.0f }, m_TextureGrass, { 1.0f,1.0f,1.0f });
			Hazel::Renderer::DrawTexturedCube({ -x * 2.0f , 10.0f, -z * 2.0f }, m_TextureSand, { 1.0f,1.0f,1.0f });
			Hazel::Renderer::DrawTexturedCube({ -x * 2.0f , 20.0f, -z * 2.0f }, m_TextureTNT, { 1.0f,1.0f,1.0f });
		}
	Hazel::Renderer::DrawSkybox(m_Skybox);
	Hazel::Renderer::EndScene();
}

void Layer3D::OnEvent(Hazel::Event& e)
{
	m_Controller.OnEvent(e);
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
