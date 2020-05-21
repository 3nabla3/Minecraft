#pragma once

#include "Hazel/Core/TimeStep.h"
#include "Hazel/Events/MouseEvent.h"
#include "PerspectiveCamera.h"
#include "glm/glm.hpp"
#include "Hazel/Core/Input.h"
#include "Hazel/Core/KeyCodes.h"

namespace Hazel {

	class PerspectiveCameraController
	{
	public:
		PerspectiveCameraController(float fov, float aspectRatio, glm::vec3 startPos = { 0.0f, 0.0f, 0.0f }, glm::vec3 startRot = { 0.0f, 0.0f, 0.0f });

		void OnUpdate(TimeStep ts);
		void OnEvent(Event& e);

		inline PerspectiveCamera& GetCamera() { return m_Camera; }
		inline const PerspectiveCamera& GetCamera() const { return m_Camera; }

		inline void SetPosition(const glm::vec3& position) { m_CameraPosition = position; }
		inline void SetRotation(const glm::vec3& rotation) { m_CameraRotation = rotation; }
	public:
		float m_CameraTranslationSpeed = 5.0f, m_CameraRotationSensitivity = 0.005f;
	private:
		bool OnMouseMoved(MouseMovedEvent& e);
	private:
		float m_AspectRatio;
		PerspectiveCamera m_Camera;

		glm::vec3 m_CameraPosition;
		glm::vec3 m_CameraRotation;

		float m_Lastx = -1.0f, m_Lasty = -1.0f;
		float m_Pitch; // how much up the player is looking in radians
	};

}