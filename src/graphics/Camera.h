#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtx/string_cast.hpp>

class Camera 
{
private:
	glm::vec3 m_Eye, m_Front, m_Up, m_Right, m_WorldUp;
	float m_Yaw, m_Pitch;
	float m_MoveSpeed, m_TurnSpeed;

	void Update();
public:
	Camera();
	Camera(glm::vec3 initEye, glm::vec3 initUp,
		float initYaw, float initPitch,
		float initMoveSpeed, float initTurnSpeed);
	~Camera();

	void KeyControl(bool* keys, float deltaTime);
	void MouseControl(float xChange, float yChange);

	glm::mat4 calculateViewMatrix();
};