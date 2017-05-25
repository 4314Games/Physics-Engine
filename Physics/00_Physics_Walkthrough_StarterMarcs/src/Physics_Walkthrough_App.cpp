#include "Physics_Walkthrough_App.h"
#include "Physics\PhysicsScene.h"
#include "Physics\PhysicsObject.h"
#include "Physics\PhysicsRenderer.h"
#include "Physics\SphereCollider.h"

#include "Gizmos.h"
#include "Input.h"
#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>

Physics_Walkthrough_App::Physics_Walkthrough_App()
{

}

Physics_Walkthrough_App::~Physics_Walkthrough_App()
{

}

bool Physics_Walkthrough_App::startup()
{
	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	aie::Gizmos::create(1000000, 1000000, 1000000, 1000000);

	// create simple camera transforms
	m_camera = new Camera();
	m_camera->SetProjection(glm::radians(45.0f), (float)getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.0f);
	m_camera->SetPosition(glm::vec3(10, 10, 10));
	m_camera->Lookat(glm::vec3(0, 0, 0));

	// Create Scene
	m_scene = new Physics::PhysicsScene();

	// Create debug Physics Renderer
	m_renderer = new Physics::PhysicsRenderer();

	// Add Objects to scene
	for (int x = 0; x < 10; x++)
	{
		for (int y = 0; y < 1; y++)
		{
			for (int z = 0; z < 10; z++)
			{
				Physics::PhysicsObject *obj = new Physics::PhysicsObject();
				obj->SetPosition(glm::vec3(x - 5.0f, y, z - 5.0f));
				obj->SetCollider(new Physics::SphereCollider(0.3f));
				m_scene->AttachObject(obj);
					m_renderer->GetRenderInfo(obj)->color = glm::vec4(
					rand() % 255 / 255.0f, // r
					rand() % 255 / 255.0f, // g
					rand() % 255 / 255.0f, // b
					0.5f				   // alpha
				);
			}
		}
	}
	
	return true;
}

void Physics_Walkthrough_App::shutdown()
{
	aie::Gizmos::destroy();
}

void Physics_Walkthrough_App::update(float deltaTime)
{
	m_camera->Update(deltaTime);

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
	float force = 5.0f;

	if (input->wasMouseButtonPressed(aie::INPUT_MOUSE_BUTTON_LEFT))
	{
		Physics::PhysicsObject *obj = new Physics::PhysicsObject();
		obj->SetMass(1000.0f);
		obj->SetBounciness(5.0f);
		obj->SetPosition(m_camera->GetPosition());
		obj->SetVelocity(m_camera->GetFront() * 20.0f);
		obj->SetCollider(new Physics::SphereCollider(0.3f));
		m_renderer->GetRenderInfo(obj)->color = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
		m_scene->AttachObject(obj);
	}

	// apply gravity to scene
	//m_scene->ApplyGlobalForce(glm::vec3(0, -9.8f, 0));

	m_scene->Update(deltaTime);
}

void Physics_Walkthrough_App::draw()
{
	// wipe the screen to the background colour
	clearScreen();

	// wipe the gizmos clean for this frame
	aie::Gizmos::clear();
	RenderGizmosGrid();

	// Create Gizmos for all the objects
	m_renderer->Draw(m_scene);

	aie::Gizmos::draw(m_camera->GetProjectionView());
}

void Physics_Walkthrough_App::RenderGizmosGrid()
{
	// draw a simple grid with gizmos
	glm::vec4 white(1);
	glm::vec4 black(0, 0, 0, 1);

	for (int i = 0; i < 21; ++i)
	{
		aie::Gizmos::addLine(glm::vec3(-10 + i, 0, 10), glm::vec3(-10 + i, 0, -10), i == 10 ? white : black);
		aie::Gizmos::addLine(glm::vec3(10, 0, -10 + i), glm::vec3(-10, 0, -10 + i), i == 10 ? white : black);
	}

	// add a transform so that we can see the axis
	aie::Gizmos::addTransform(glm::mat4());
}
