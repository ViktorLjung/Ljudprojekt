#pragma once
#include "PrecompiledHeader.h"

#include "SoundSystem.h"
#include "SFX.h"
#include "BGM.h"

class Camera
{
public:

	Camera(float fov, float AspectRatio, float nearClip, float farClip);
	glm::vec3 Position() const { return m_Position; }
	void Position(glm::vec3 val) { m_Position = val; }
	float Pitch() const { return m_Pitch; }
	void Pitch(float val) { m_Pitch = val; }
	float Yaw() const { return m_Yaw; }
	void Yaw(float val) { m_Yaw = val; }
	float Roll() const { return m_Yaw; }
	void Roll(float val) { m_Yaw = val; }
	float MoveSpeed() const { return m_MoveSpeed; }
	void MoveSpeed(float val) { m_MoveSpeed = val; }
	glm::mat4 projectionMatrix();
	glm::mat4 viewMatrix();
	glm::vec3 Forward();
	glm::vec3 Right();
	glm::vec3 Up();
	void Input(GLFWwindow* window, double dt, int windowWidth, int windowHeight);

private:
	float m_NearClip;
	float m_FarClip;
	float m_AspectRatio;
	float m_FOV;

	glm::vec3 m_Position;
	float m_MoveSpeed;
	float m_Pitch;
	float m_Roll;
	float m_Yaw;

	SFX bazinga;
	int m_LastKeyPress;
};
