#include "hzpch.h"
#include "OrthographicCameraController.h"

#include "Hazel/Core/Input.h"
#include "Hazel/Core/KeyCodes.h"


namespace Hazel {

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio), m_Camera(-aspectRatio * m_ZoomLevel, aspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_RotationEnabled(rotation)
	{
	}

	void OrthographicCameraController::OnUpdate(TimeStep ts)
	{
		HZ_PROFILE_FUNCTION();
		if (Input::IsKeyPressed(HZ_KEY_A))
			m_CameraPosition.x -= m_CameraTranslationSpeed * ts;

		if (Input::IsKeyPressed(HZ_KEY_D))
			m_CameraPosition.x += m_CameraTranslationSpeed * ts;

		if (Input::IsKeyPressed(HZ_KEY_S))
			m_CameraPosition.y -= m_CameraTranslationSpeed * ts;

		if (Input::IsKeyPressed(HZ_KEY_W))
			m_CameraPosition.y += m_CameraTranslationSpeed * ts;

		if (m_RotationEnabled)
		{
			if (Input::IsKeyPressed(HZ_KEY_UP))
				m_CameraRotation.x += m_CameraRotationSpeed * ts;

			if (Input::IsKeyPressed(HZ_KEY_DOWN))
				m_CameraRotation.x -= m_CameraRotationSpeed * ts;

			if (Input::IsKeyPressed(HZ_KEY_LEFT))
				m_CameraRotation.y += m_CameraRotationSpeed * ts;

			if (Input::IsKeyPressed(HZ_KEY_RIGHT))
				m_CameraRotation.y -= m_CameraRotationSpeed * ts;

			if (Input::IsKeyPressed(HZ_KEY_Q))
				m_CameraRotation.z += m_CameraRotationSpeed * ts;

			if (Input::IsKeyPressed(HZ_KEY_E))
				m_CameraRotation.z -= m_CameraRotationSpeed * ts;

			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);
		
		m_CameraTranslationSpeed = m_ZoomLevel;
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		HZ_PROFILE_FUNCTION();
		EventDispacher dispacher(e);
		dispacher.Dispach<MouseScrolledEvent>(HZ_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispacher.Dispach<WindowResizeEvent>(HZ_BIND_EVENT_FN(OrthographicCameraController::OnWindowResize));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		HZ_PROFILE_FUNCTION();
		m_ZoomLevel -= e.GetYOffset() * 0.1f;
		m_ZoomLevel = (float)std::clamp(m_ZoomLevel, 0.01f, 15.0f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnWindowResize(WindowResizeEvent& e)
	{
		HZ_PROFILE_FUNCTION();
		float yScale = e.GetHeight() / 720.0f;
		m_AspectRatio = yScale * (float)e.GetWidth() / e.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -yScale * m_ZoomLevel, yScale * m_ZoomLevel);
		return false;
	}

}
