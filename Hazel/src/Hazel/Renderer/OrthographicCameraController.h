#pragma once

#include "Hazel/Renderer/OrthographicCamera.h"
#include "Hazel/Core/TimeStep.h"

#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/MouseEvent.h"

namespace Hazel {

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false); 

		void OnUpdate(TimeStep ts);
		void OnEvent(Event& e);

		inline OrthographicCamera& GetCamera() { return m_Camera; }
		inline const OrthographicCamera& GetCamera() const { return m_Camera; }

		float GetZoomLevel() const { return m_ZoomLevel; }
		void SetZoomLevel(float level) { m_ZoomLevel = level; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthographicCamera m_Camera;

		bool m_RotationEnabled;
		glm::vec3 m_CameraPosition = glm::vec3(0.0f);
		glm::vec3 m_CameraRotation = glm::vec3(0.0f);

		float m_CameraTranslationSpeed = 5.0f, m_CameraRotationSpeed = 3.1415f;
	};

}