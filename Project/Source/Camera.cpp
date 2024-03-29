#include "Camera.h"



Camera::Camera(float fov, float AspectRatio, float nearClip, float farClip)
{
	m_Position = glm::vec3();
	m_Pitch = 0;
	m_Yaw = 0;
	m_Roll = 0;
	m_MoveSpeed = 10;
	m_AspectRatio = AspectRatio;
	m_FOV = fov;
	m_NearClip = nearClip;
	m_FarClip = farClip;

	//song = &b1;
}

glm::mat4 Camera::projectionMatrix()
{
	return glm::perspective(m_FOV, m_AspectRatio, m_NearClip, m_FarClip);
};

glm::mat4 Camera::viewMatrix()
{
	glm::mat4 view = glm::mat4();
	view = glm::rotate(view, m_Pitch, glm::vec3(1.f, 0.f, 0.f));	//Pitch
	view = glm::rotate(view, m_Yaw, glm::vec3(0.f, 1.f, 0.f));	//Yaw
	view = glm::rotate(view, m_Roll, glm::vec3(0.f, 0.f, 1.f));	//Roll
	view = glm::translate(view, -m_Position);		//Position

	return view;
};

glm::vec3 Camera::Forward()
{
	glm::mat4 Orientation;
	Orientation = glm::rotate(Orientation, m_Pitch, glm::vec3(1, 0, 0));
	Orientation = glm::rotate(Orientation, m_Yaw, glm::vec3(0, 1, 0));
	
	return glm::vec3(glm::vec4(0, 0, -1, 0) * Orientation);
}

glm::vec3 Camera::Right()
{
	glm::mat4 Orientation;
	Orientation = glm::rotate(Orientation, m_Pitch, glm::vec3(1, 0, 0));
	Orientation = glm::rotate(Orientation, m_Yaw, glm::vec3(0, 1, 0));

	return glm::vec3(glm::vec4(1, 0, 0, 0) * Orientation);
}

glm::vec3 Camera::Up()
{
	glm::mat4 Orientation;
	Orientation = glm::rotate(Orientation, m_Pitch, glm::vec3(1, 0, 0));
	Orientation = glm::rotate(Orientation, m_Yaw, glm::vec3(0, 1, 0));

	return glm::vec3(glm::vec4(0, 1, 0, 0) * Orientation);
}

void Camera::Input(GLFWwindow* window, double dt, int windowWidth, int windowHeight)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		m_Position += Forward() * m_MoveSpeed * (float)dt;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		m_Position -= Forward() * m_MoveSpeed * (float)dt;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		m_Position += Right() * m_MoveSpeed * (float)dt;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		m_Position -= Right() * m_MoveSpeed * (float)dt;
	}

// 	//Beats
// 	if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
// 	{
// 			song->StopSound();
// 			b1.PlaySound(0.1);
// 			song = &b1;
// 	}
// 	if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
// 	{
// 		
// 			song->StopSound();
// 			b2.PlaySound(0.1);
// 			song = &b2;
// 	}
// 	if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS)
// 	{
// 			song->StopSound();
// 			b3.PlaySound(0.1);
// 			song = &b3;
// 	}
// 	if (glfwGetKey(window, GLFW_KEY_F4) == GLFW_PRESS)
// 	{
// 			song->StopSound();
// 			b4.PlaySound(0.1);
// 			song = &b4;
// 	}
// 	if (glfwGetKey(window, GLFW_KEY_F5) == GLFW_PRESS)
// 	{
// 			song->StopSound();
// 			b5.PlaySound(0.1);
// 			song = &b5;
// 	}

	static double mousePosX, mousePosY;

	glfwGetCursorPos(window, &mousePosX, &mousePosY);
	double deltaX, deltaY;
	deltaX = mousePosX - (float)windowWidth / 2;
	deltaY = mousePosY - (float)windowHeight / 2;

	m_Pitch += deltaY / 300;
	m_Yaw += deltaX / 300;


	if (m_Pitch > 3.14f / 2)
	{
		m_Pitch = 3.14f / 2;
	}
	else if (m_Pitch < -3.14f / 2)
	{
		m_Pitch = -3.14f / 2;
	}
	glfwSetCursorPos(window, windowWidth / 2, windowHeight / 2);

	m_LastKeyPress = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
}


