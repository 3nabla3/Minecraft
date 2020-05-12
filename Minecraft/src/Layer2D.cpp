#include "Layer2D.h"

Layer2D::Layer2D()
	:Layer("Layer2D"), m_Controller(1.778f, false)
{
	m_CrosshairVertexArray = Hazel::VertexArray::Create();
	float length = 0.05f;
	float width = 0.005f;

	float crosshairVertices[] = {
		-length,  width,
		-length, -width,
		-width, -length,
		 width, -length,
		 length, -width,
		 length,  width,
		 width,  length,
		-width,  length,
	};
	auto vertexBuffer = Hazel::VertexBuffer::Create(crosshairVertices, sizeof(crosshairVertices));
	vertexBuffer->SetLayout({
		{Hazel::ShaderDataType::Float2, "a_Position"}
		});
	m_CrosshairVertexArray->AddVertexBuffer(vertexBuffer);

	uint32_t crosshairIndices[] = {
		0, 1, 5, 1, 5, 4,
		7, 2, 6, 2, 6, 3
	};
	auto indexBuffer = Hazel::IndexBuffer::Create(crosshairIndices, sizeof(crosshairIndices) / sizeof(uint32_t));
	m_CrosshairVertexArray->SetIndexBuffer(indexBuffer);

	m_CrosshairShader = Hazel::Shader::Create("assets/shaders/Crosshair.glsl");
}

void Layer2D::OnUpdate(Hazel::TimeStep ts)
{
	m_CrosshairShader->Bind();
	m_CrosshairShader->SetMat4("u_Projection", m_Controller.GetCamera().GetProjectionMatrix());

	Hazel::Renderer2D::BeginScene(m_Controller.GetCamera());
	Hazel::Renderer2D::Submit(m_CrosshairVertexArray, m_CrosshairShader);
	Hazel::Renderer2D::EndScene();
}

void Layer2D::OnEvent(Hazel::Event& e)
{
}