#include "Layer3D.h"

Layer3D::Layer3D()
	:Layer("Layer 3D"), m_Controller(45.0f, 1.778f)
{
	m_Texture = Hazel::TextureCubeMap::Create({
		"assets/textures/grass_side.jpg",
		"assets/textures/grass_side.jpg",
		"assets/textures/grass_top.jpg",
		"assets/textures/grass_bottom.jpg",
		"assets/textures/grass_side.jpg",
		"assets/textures/grass_side.jpg"
		});

	auto skybox = Hazel::TextureCubeMap::Create({
		"assets/skybox/right.jpg",
		"assets/skybox/left.jpg",
		"assets/skybox/top.jpg",
		"assets/skybox/bottom.jpg",
		"assets/skybox/front.jpg",
		"assets/skybox/back.jpg"
		});

	Hazel::Renderer::SetSkybox(skybox);
}

void Layer3D::OnUpdate(Hazel::TimeStep ts)
{
	m_Controller.OnUpdate(ts);

	Hazel::RenderCommand::Clear();

	Hazel::Renderer::BeginScene(m_Controller.GetCamera());
	for (int x = 0; x < 10; x++)
		for (int z = 0; z < 10; z++)
			Hazel::Renderer::DrawTexturedCube({ x * 2.0f ,0.0f, z * 2.0f}, m_Texture, { 1.0f,1.0f,1.0f });
	Hazel::Renderer::EndScene();
}

void Layer3D::OnEvent(Hazel::Event& e)
{
	m_Controller.OnEvent(e);
}