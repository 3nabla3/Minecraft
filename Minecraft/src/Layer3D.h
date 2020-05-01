#pragma once
#include <Hazel.h>

class Layer3D : public Hazel::Layer
{
public:
	Layer3D();
	void OnUpdate(Hazel::TimeStep ts);
	void OnEvent(Hazel::Event& e);

private:
	Hazel::PerspectiveCameraController m_Controller;
	Hazel::Ref<Hazel::TextureCubeMap> m_Texture;

	float m_LastX = -1.0f, m_LastY = -1.0f;
};