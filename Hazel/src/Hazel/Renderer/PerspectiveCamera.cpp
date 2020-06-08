#include "hzpch.h"
#include "PerspectiveCamera.h"

namespace Hazel {

	PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio)
		:m_ProjectionMatrix(glm::perspective(fov, aspectRatio, 0.1f, 1500.0f)), m_ViewMatrix(1.0f), m_Fov(fov)
	{
		m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void PerspectiveCamera::SetProjection(float fov, float aspectRatio)
	{
		m_Fov = fov;
		m_ProjectionMatrix = glm::perspective(fov, aspectRatio, 0.1f, 150.0f);
		m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void PerspectiveCamera::RecalculateViewMatrix()
	{
		using namespace glm;
		mat4 transform = translate(mat4(1.0f), m_Position) *
			rotate(mat4(1.0f), m_Rotation.y, vec3(0, 1, 0)) * // in this order because the y rot needs to be applied first
			rotate(mat4(1.0f), m_Rotation.z, vec3(0, 0, 1)) * // the z rot is rarely used
			rotate(mat4(1.0f), m_Rotation.x, vec3(1, 0, 0));

		m_ViewMatrix = inverse(transform);
		m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}