#include "Layer3D.h"
#include "PerlinNoise.hpp"
#include "Chunk.h"
#include <fstream>

Layer3D::Layer3D()
	:Layer("Layer 3D"), m_Controller(45.0f, 1.778f, { 0.0f, 3.0f, 0.0f })
{
	// load the values for debugging from disk
	std::string settings;
	std::ifstream in("minecraftSettings.txt", std::ios::in);
	if (in.is_open())
	{
		std::getline(in, settings);
		size_t index = settings.find("=") + 2;
		HZ_TRACE("{0}", settings.substr(index));
		m_SliderVal1 = std::stof(settings.substr(index));
		std::getline(in, settings);
		index = settings.find("=") + 2;
		m_SliderVal2 = std::stof(settings.substr(index));
	}
	in.close();

	m_TextureDirt = UploadTexture("dirt");
	m_TextureSand = UploadTexture("sand");
	m_TextureGrass = UploadTexture("grass");
	m_TextureOak = UploadTexture("oak");

	m_Skybox = Hazel::TextureCubeMap::Create({
		"assets/skybox/right.jpg",
		"assets/skybox/left.jpg",
		"assets/skybox/top.jpg",
		"assets/skybox/bottom.jpg",
		"assets/skybox/front.jpg",
		"assets/skybox/back.jpg"
		});

	m_Controller.m_CameraTranslationSpeed = 50.0f;
	m_Terrain = Terrain(300, 300, m_TextureGrass);
}

void Layer3D::OnUpdate(Hazel::TimeStep ts)
{
	m_FrameTime = ts.GetMiliseconds();
	if (m_EnableMovements)
		m_Controller.OnUpdate(ts);

	Hazel::RenderCommand::SetClearColor({ 0.0f, 1.0f, 0.0f, 1.0f });
	Hazel::RenderCommand::Clear();
	Hazel::Renderer::ResetStats();

	Hazel::Renderer::BeginScene(m_Controller.GetCamera());
	//Hazel::Renderer::DrawColoredCube({ -2, -2, -2 }, { 1, 1, 1, 1 }, { 1, 1, 1 });
	m_Terrain.Render(m_SliderVal1, m_SliderVal2);
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
	SliderFloat("Val 1", &m_SliderVal1, 0, 100);
	SliderFloat("Val 2", &m_SliderVal2, 0, 100);
	End();

	Begin("Cam info");
	Text("Position");
	Text("Camera X-Position %f units", m_Controller.GetCamera().GetPosition().x);
	Text("Camera Y-Position %f units", m_Controller.GetCamera().GetPosition().y);
	Text("Camera Z-Position %f units", m_Controller.GetCamera().GetPosition().z);
	Text("Rotation");
	Text("Camera X-Rotation %f rad", m_Controller.GetCamera().GetRotation().x);
	Text("Camera Y-Rotation %f rad", m_Controller.GetCamera().GetRotation().y);
	Text("Camera Z-Rotation %f rad", m_Controller.GetCamera().GetRotation().z);
	End();

	// write the values for debugging to disk
	std::ofstream in("minecraftSettings.txt", std::ios::out);
	if (in.is_open())
	{
		in << "SliderVal1 = " << m_SliderVal1 << "\n";
		in << "SliderVal2 = " << m_SliderVal2;
	}
	in.close();
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
