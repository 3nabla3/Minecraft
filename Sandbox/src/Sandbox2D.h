#pragma once
#include "Hazel.h"

class Sandbox2D : public Hazel::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Hazel::TimeStep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Hazel::Event& e) override;
private:
	Hazel::OrthographicCameraController m_CameraController;

	Hazel::Ref<Hazel::VertexArray> m_SquareVA;
	Hazel::Ref<Hazel::Shader> m_FlatColorShader;

	Hazel::Ref<Hazel::Texture2D> m_Texture;

	glm::vec4 m_SquareColor = glm::vec4(0.8f, 0.3f, 0.2f, 1.0f);
	glm::vec2 m_SquarePosition = { 0.0f, 0.0f };
	float m_SquareRotation = 0.0f;
	glm::vec2 m_SquareSize = { 1.0f, 1.0f };
	
	glm::vec4 m_TextureColor = glm::vec4(1.0f);
	glm::vec2 m_TexturePosition = { 0.0f, 0.0f };
	float m_TextureRotation = 0.0f;
	glm::vec2 m_TextureSize = { 10.0f, 10.0f };

	struct ProfileResult
	{
		const char* Name;
		float Time;
	};

	std::vector<ProfileResult> m_ProfileResults;
};
