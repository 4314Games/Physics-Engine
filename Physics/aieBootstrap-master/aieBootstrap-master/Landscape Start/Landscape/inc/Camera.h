#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <Application.h>

class Camera
{
public:
	Camera(aie::Application* parent);
	~Camera();

	void Update(float p_deltaTime);

	void LookAt(glm::vec3 target);
	void SetPosition(glm::vec3 position);

	 glm::mat4 & GetView();
	const glm::vec3 & GetPos()	{ return m_position; }

	const glm::mat4 & GetReverseView();
	
	float m_yaw = 0.0f;		//left/right rortation of view
	glm::vec3 m_position;

	bool isVisible(glm::vec3 position, glm::vec3 furthestPos);
	glm::mat4 getProjectionMatrix() { return m_projectionMatrix; }

private:
	aie::Application* app;
	
	glm::mat4 m_viewMatrix;		//the matrix well pass to opengl so it knows how the camera looks at the world
	
	glm::mat4 m_projectionMatrix;
	
	glm::vec3 m_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);	//setting camera orientation so that the y	
	
	glm::vec3 m_cameraLook;	// the actuale direction the camera is aiming
	
	float m_pitch = 0.0f;	//up/down raotation
	float m_roll = 0.0f;

	const float m_mouseSenstivity = 10.0f;
	float m_moveSpeed = 3.0f;

	int m_lastMouseXPos = 0;
	int m_lastMouseYpos = 0;

	void CalculateLook();	//gives is the vector of dircetion using yaw pitch and roll
	void CalculateView();	//sets up the view matrix based on our camera information
	void CalculateProjection();
};          



