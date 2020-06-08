#include "hzpch.h"
#include "PerspectiveCameraController.h"

namespace Hazel {

	PerspectiveCameraController::PerspectiveCameraController(float fov, float aspectRatio, glm::vec3 startPos, glm::vec3 startRot)
		:m_Camera(fov, aspectRatio), m_AspectRatio(aspectRatio), m_Pitch(0.0f), m_CameraPosition(startPos), m_CameraRotation(startRot)
	{
		m_CameraRotation.y += 3.1415f / 4.0f;
	}

	void PerspectiveCameraController::OnUpdate(TimeStep ts)
	{
		if (Hazel::Input::IsKeyPressed(HZ_KEY_D))
		{
			m_CameraPosition.z += glm::cos(m_CameraRotation.y + 3.1415f / 2.0f) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.x += glm::sin(m_CameraRotation.y + 3.1415f / 2.0f) * m_CameraTranslationSpeed * ts;
		}

		if (Hazel::Input::IsKeyPressed(HZ_KEY_A))
		{
			m_CameraPosition.z += glm::cos(m_CameraRotation.y - 3.1415f / 2.0f) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.x += glm::sin(m_CameraRotation.y - 3.1415f / 2.0f) * m_CameraTranslationSpeed * ts;
		}

		if (Hazel::Input::IsKeyPressed(HZ_KEY_W))
		{
			m_CameraPosition.z -= glm::cos(m_CameraRotation.y) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.x -= glm::sin(m_CameraRotation.y) * m_CameraTranslationSpeed * ts;
		}

		if (Hazel::Input::IsKeyPressed(HZ_KEY_S))
		{
			m_CameraPosition.z += glm::cos(m_CameraRotation.y) * m_CameraTranslationSpeed * ts;
			m_CameraPosition.x += glm::sin(m_CameraRotation.y) * m_CameraTranslationSpeed * ts;
		}

		if (Hazel::Input::IsKeyPressed(HZ_KEY_SPACE))
			m_CameraPosition.y += m_CameraTranslationSpeed * ts;

		if (Hazel::Input::IsKeyPressed(HZ_KEY_LEFT_SHIFT))
			m_CameraPosition.y -= m_CameraTranslationSpeed* ts;

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);
	}

	void PerspectiveCameraController::OnEvent(Event& e)
	{
		EventDispacher dispacher(e);
		dispacher.Dispach<MouseMovedEvent>(HZ_BIND_EVENT_FN(PerspectiveCameraController::OnMouseMoved));
	}

	bool PerspectiveCameraController::OnMouseMoved(MouseMovedEvent& e)
	{
		if (m_Lastx == -1.0f || m_Lasty == -1.0f)
		{
			m_Lastx = e.GetX();
			m_Lasty = e.GetY();
		}
		float xOffset = m_Lastx - e.GetX();
		float yOffset = e.GetY() - m_Lasty;

		m_Lastx = e.GetX();
		m_Lasty = e.GetY();

		m_CameraRotation.y += xOffset * m_CameraRotationSensitivity;
		m_CameraRotation.x -= yOffset * m_CameraRotationSensitivity;

		m_CameraRotation.x = std::clamp(m_CameraRotation.x, -3.1415f / 2.0f, 3.1415f / 2.0f);

		if (m_CameraRotation.y > 3.1415f * 2) m_CameraRotation.y -= 3.1415f * 2;
		if (m_CameraRotation.y < 0) m_CameraRotation.y += 3.1415f * 2;

		return false;
	}

}