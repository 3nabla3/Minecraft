#pragma once
#include <Hazel.h>

class Layer2D : public Hazel::Layer
{
public:
	Layer2D();
	void OnUpdate(Hazel::TimeStep ts);
	void OnEvent(Hazel::Event& e);
private:
	Hazel::OrthographicCameraController m_Controller;

	Hazel::Ref<Hazel::VertexArray> m_CrosshairVertexArray;
	Hazel::Ref<Hazel::Shader> m_CrosshairShader;
};
