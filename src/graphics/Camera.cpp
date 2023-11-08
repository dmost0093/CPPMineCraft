#include "Camera.h"


Camera::Camera()
{
}

Camera::Camera(glm::vec3 initEye, glm::vec3 initUp, float initYaw, float initPitch, float initMoveSpeed, float initTurnSpeed)
	: m_Eye{ initEye }, m_Front{ glm::vec3{0.0f,0.0f,-1.0f} }, m_WorldUp{ initUp },
	m_Pitch{ initPitch }, m_Yaw{ initYaw },
	m_MoveSpeed{ initMoveSpeed }, m_TurnSpeed{ initTurnSpeed }
{
	Update();
}

Camera::~Camera()
{
}

void Camera::KeyControl(bool* keys, float deltaTime)
{
	// movespeed = the speed of the camera when we moving camera with w,a,s,d,
	// deltiTime is the time(real time) difference between current frame and last frame
	float velocity = m_MoveSpeed * deltaTime;

	if (keys[GLFW_KEY_W])
	{
		m_Eye += m_Front * velocity;
	}
	if (keys[GLFW_KEY_S])
	{
		m_Eye -= m_Front * velocity;
	}
	if (keys[GLFW_KEY_D])
	{
		m_Eye += m_Right * velocity;
	}
	if (keys[GLFW_KEY_A])
	{
		m_Eye -= m_Right * velocity;
	}
}

void Camera::MouseControl(float xChange, float yChange)
{
	// How much the rotate the angle of camera with 1 pixel of mouse movement from the window
	xChange *= m_TurnSpeed;
	yChange *= m_TurnSpeed;

	m_Yaw += xChange;
	m_Pitch += yChange;

	// Maximum (y) pitch
	if (m_Pitch > 89.0f)
		m_Pitch = 89.0f;

	if (m_Pitch < -89.0f)
		m_Pitch = -89.0f;

	Update();
}

glm::mat4 Camera::calculateViewMatrix()
{
	glm::mat4 view = glm::lookAt(m_Eye, m_Eye + m_Front, m_Up);
	//std::cout << glm::to_string(view) << std::endl;
	return glm::lookAt(m_Eye, m_Eye + m_Front, m_Up); //At = Eye + Front
}

void Camera::Update()
{
	// Calculating the Change in the Front Vector Based on Mouse Input (Spherical Coordinates to Cartesian Coordinates)
	m_Front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	m_Front.y = sin(glm::radians(m_Pitch));
	m_Front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
	m_Front = glm::normalize(m_Front);

	m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
	m_Up = glm::normalize(glm::cross(m_Right, m_Front));
}
