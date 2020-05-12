#pragma once
#include <Hazel.h>
#include <imgui/imgui.h>

class Layer3D : public Hazel::Layer
{
public:
	Layer3D();
	void OnUpdate(Hazel::TimeStep ts) override;
	void OnEvent(Hazel::Event& e) override;
	void OnImGuiRender() override;
	static Hazel::Ref<Hazel::TextureCubeMap> UploadTexture(const std::string& filepath);

private:
	Hazel::PerspectiveCameraController m_Controller;

	Hazel::Ref<Hazel::TextureCubeMap> m_Skybox;

	Hazel::Ref<Hazel::TextureCubeMap> m_TextureDirt;
	Hazel::Ref<Hazel::TextureCubeMap> m_TextureSand;
	Hazel::Ref<Hazel::TextureCubeMap> m_TextureTNT;

	float m_LastX = -1.0f, m_LastY = -1.0f;
};