#include "hzpch.h"
#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Hazel {

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(1.0f)
	{
		HZ_PROFILE_FUNCTION();
		m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		HZ_PROFILE_FUNCTION();
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -10.0f, 10.0f);
		m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		HZ_PROFILE_FUNCTION();
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::rotate(glm::mat4(1.0f), m_Rotation.x, glm::vec3(1, 0, 0)) *
			glm::rotate(glm::mat4(1.0f), m_Rotation.y, glm::vec3(0, 1, 0)) *
			glm::rotate(glm::mat4(1.0f), m_Rotation.z, glm::vec3(0, 0, 1));


		m_ViewMatrix = glm::inverse(transform);
		m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

}