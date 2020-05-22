#include "Layer3D.h"
#include "PerlinNoise.hpp"

Layer3D::Layer3D()
	:Layer("Layer 3D"), m_Controller(45.0f, 1.778f, { 0.0f, 3.0f, 0.0f }), m_Smoothness(150.0f), m_MaxHeight(20.0f)
{
	m_TextureDirt = UploadTexture("dirt");
	m_TextureSand = UploadTexture("sand");
	m_TextureGrass = UploadTexture("grass");

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
	m_FrameTime = ts.GetMiliseconds();

	const int size = 300;

	if (m_EnableMovements)
		m_Controller.OnUpdate(ts);

	siv::BasicPerlinNoise<float> gen(0);

	Hazel::RenderCommand::SetClearColor({ 0.0f, 1.0f, 0.0f, 1.0f });
	Hazel::RenderCommand::Clear();
	Hazel::Renderer::ResetStats();

	Hazel::Renderer::BeginScene(m_Controller.GetCamera());
	for (uint32_t i = 0; i < size; i++)
		for (uint32_t j = 0; j < size; j++)
		{
			float result = gen.noise2D_0_1(i / m_Smoothness, j / m_Smoothness) * m_MaxHeight;
			//HZ_TRACE("{0}", result);
			int height = (int)result;
			Hazel::Ref<Hazel::TextureCubeMap> texture;
			glm::vec4 tintColor;

			for (int32_t k = 0; k < height; k += 2)
			{
				if (k < m_MaxHeight * 0.33) { texture = m_TextureDirt; tintColor = { 1.0f, 1.0f, 1.0f, 1.0f }; }
				else if (k < m_MaxHeight * 0.66) { texture = m_TextureGrass; tintColor = { 1.0f, 1.0f, 1.0f, 1.0f }; }
				else { texture = m_TextureSand; tintColor = { 1.0f, 1.0f, 1.0f, 1.0f }; }
				Hazel::Renderer::DrawTexturedCube({ i * 2.0f, k, j * 2.0f }, texture, { 1.0f, 1.0f, 1.0f }, tintColor);
			}
		}
	Hazel::Renderer::EndScene();
	Hazel::Renderer::DrawSkybox(m_Skybox);
}

bool Layer3D::OnKeyPressed(Hazel::KeyPressedEvent& e)
{
	if (e.GetKeyCode() == HZ_KEY_ESCAPE)
		m_EnableMovements = !m_EnableMovements;

	return false;
}

void Layer3D::OnEvent(Hazel::Event& e)
{
	if (m_EnableMovements)
		m_Controller.OnEvent(e);

	Hazel::EventDispacher dispacher(e);

	dispacher.Dispach<Hazel::KeyPressedEvent>(HZ_BIND_EVENT_FN(Layer3D::OnKeyPressed));
}

void Layer3D::OnImGuiRender()
{
	using namespace ImGui;
	auto stats = Hazel::Renderer::GetStats();

	Begin("Render stats");
	Text("Draw calls %d", stats.DrawCalls);
	Text("Cube count %d", stats.CubeCount);
	Text("Frame time %f ms", m_FrameTime);
	End();

	Begin("Change noise Settings");
	SliderFloat("Max Height", &m_MaxHeight, 1, 100);
	SliderFloat("Smoothness", &m_Smoothness, 5, 300);
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
