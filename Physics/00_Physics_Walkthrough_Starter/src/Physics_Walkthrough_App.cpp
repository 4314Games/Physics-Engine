#include "Physics_Walkthrough_App.h"
#include "Physics\PhysicsRenderer.h"
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Camera.h"
#include "Physics\SphereCollider.h"

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
	aie::Gizmos::create(10000, 10000, 10000, 10000);

	// create simple camera transforms
	m_camera = new Camera();
	m_camera->SetProjection(glm::radians(45.0f), (float)getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.0f);
	m_camera->SetPosition(glm::vec3(10, 10, 10));
	m_camera->Lookat(glm::vec3(0, 0, 0));

	//Create scene
	m_scene = new Physics::PhysicScene();	

	//Create Debug physics renderer
	m_renderer = new Physics::PhysicsRenderer();

	//Add objects to scene
	for (int x = 0; x < 10; x++)
	{
		Physics::PhysicsObject *object = new Physics::PhysicsObject();
		//Set Collider
		object->SetPos(glm::vec3(x, x * 10, 0));
		object->SetCollider(new Physics::SphereCollider(1.0f));
		m_scene->AttachObject(object);
		//Renders random color
		m_renderer->GetRenderInfro(object)->color = glm::vec4(rand() % 255 / 255.0f, rand() % 255 / 255.0f, rand() % 255 / 255.0f,1.0f);
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

	float force = 10;
	//if (input->isKeyDown(aie::INPUT_KEY_LEFT))m_objectOne->ApplyForce(glm::vec3(-force, 0, 0));
	//if (input->isKeyDown(aie::INPUT_KEY_RIGHT))m_objectOne->ApplyForce(glm::vec3(force, 0, 0));
	//if (input->isKeyDown(aie::INPUT_KEY_UP))m_objectOne->ApplyForce(glm::vec3(0, force, 0));
	//if (input->isKeyDown(aie::INPUT_KEY_DOWN))m_objectOne->ApplyForce(glm::vec3(0, -force, 0));

	//Apply scene grabity
	m_scene->ApplyGlobalForce(glm::vec3(0, -9.8f, 0));
	
	//Updates Scene Physics
	m_scene->Update(deltaTime);

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void Physics_Walkthrough_App::draw()
{
	// wipe the screen to the background colour
	clearScreen();

	// wipe the gizmos clean for this frame
	aie::Gizmos::clear();
	RenderGizmosGrid();
	//RenderGizmosPhysics();
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

