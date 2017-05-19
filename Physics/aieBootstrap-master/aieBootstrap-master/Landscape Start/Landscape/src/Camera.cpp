#define GLM_SWIZZLE
#include "Camera.h"
#include <Input.h>
#include <glm\glm.hpp>
#include <glm\ext.hpp>


Camera::Camera(aie::Application* parent)
{
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f,
		parent->getWindowWidth() / (float)parent->getWindowHeight(),
		0.1f, 1000.f);
	app = parent;
}

Camera::~Camera()
{
}

void Camera::Update(float p_deltaTime)		
{
	//Rotate camera base on mouse movement
	aie::Input *input = aie::Input::getInstance();
	if (input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_RIGHT))
	{
		float rotationAmound = m_mouseSenstivity * p_deltaTime;
		int mouseX, mouseY;		//mouse X Y cords
		float xOffset, yOffset;	//how far we want to rotate the camera in these axes
		input->getMouseXY(&mouseX, &mouseY);	//get the mouse location and place it in our variables.

		xOffset = (mouseX - m_lastMouseXPos) * rotationAmound;
		yOffset = (mouseY - m_lastMouseYpos) * rotationAmound;

		m_lastMouseXPos = mouseX;
		m_lastMouseYpos = mouseY;

		//use these values ion our yaw and pitch values
		m_yaw += xOffset;
		m_pitch += yOffset;

		CalculateLook();
		//move camera
		if (input->isKeyDown(aie::INPUT_KEY_W))
			m_position += m_moveSpeed * p_deltaTime * m_cameraLook;
		if (input->isKeyDown(aie::INPUT_KEY_S))
			m_position -= m_moveSpeed * p_deltaTime * m_cameraLook;

		glm::vec3 strafeVec = glm::normalize(glm::cross(m_cameraLook, m_cameraUp));

		if (input->isKeyDown(aie::INPUT_KEY_A))
			m_position -= m_moveSpeed * p_deltaTime * strafeVec;
		if (input->isKeyDown(aie::INPUT_KEY_D))
			m_position += m_moveSpeed * p_deltaTime * strafeVec;

		if (input->isKeyDown(aie::INPUT_KEY_LEFT_SHIFT))	//If Shift down move faster
		{
			m_moveSpeed  =  10.0f;
		}
		else if (input->isKeyUp(aie::INPUT_KEY_LEFT_SHIFT))
		{
			m_moveSpeed = 5.0f;
		}
	}
	else
	{
		input->getMouseXY(&m_lastMouseXPos, &m_lastMouseYpos);
	}	
}

void Camera::LookAt(glm::vec3 target)
{
	glm::vec3 look = glm::normalize(target - m_position);	//create a vector which is the direction to the ta
	m_pitch = glm::degrees(glm::asin(look.y));
	m_yaw = glm::degrees(atan2(look.y, look.x));
	CalculateLook();
}

void Camera::SetPosition(glm::vec3 position)
{
	m_position = position;
	CalculateView();
}

 glm::mat4 & Camera::GetView()
{
	CalculateLook();
	return m_viewMatrix;
}

const glm::mat4 & Camera::GetReverseView()
{
	return m_viewMatrix = glm::lookAt(-m_position,- m_position - m_cameraLook, -m_cameraUp);
}

void Camera::CalculateLook()
{
	//some circle geometry maths to convert the viewing angle from
	// yaw, pitch and roll into a single normalised vector
	glm::vec3 look;
	look.x = glm::cos(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));
	look.y = glm::sin(glm::radians(m_pitch)) * glm::cos(glm::radians(m_roll));
	look.z = glm::sin(glm::radians(m_yaw)) * glm::cos(glm::radians(m_pitch));
	m_cameraLook = glm::normalize(look);
	CalculateView();
}

void Camera::CalculateView()
{
	m_viewMatrix = glm::lookAt(m_position, m_position + m_cameraLook, m_cameraUp);
}

void Camera::CalculateProjection()
{
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f,
		app->getWindowWidth() / (float)app->getWindowHeight(),
		0.1f, 1000.f);
}

bool Camera::isVisible(glm::vec3 position, glm::vec3 furthestPos)
{
	//were using the centre position of an object as well as \
	// another position which is on a theoretical sphere that
	//surronds the object

	//Step 1: Transform centre and surface positions into scree space
	//using the projecion matrix
	glm::mat4 projView = m_projectionMatrix * m_viewMatrix;
	glm::vec4 pos(position.x, position.y, position.z, 1);
	furthestPos = (projView * glm::vec4(furthestPos, 1)).xyz;

	//Step 2: Now that our sphere is in screen space we can test it
	//againstt the frustum cube
	glm::vec3 radial = position - furthestPos;
	float radius = radial.length();

	//Now test the centre and add/remove the radius and see if its ever within
	// -1 and 1(so would be drawn)

	//if (position.x <= 1 && position.x >= -1 &&
	//	position.y <= 1 && position.y >= -1 &&
	//	position.z <= 1 && position.z >= -1)
	//{
	//	return true;	//the center is inside the viewable area
	//}

	for (int i = 0; i < 3; i++)
	{
		if (abs(position[i]) >= 1)	//if we definatly outside
			return false;		
	}
	return true;
}