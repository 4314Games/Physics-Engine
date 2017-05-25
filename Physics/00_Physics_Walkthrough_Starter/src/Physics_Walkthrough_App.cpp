#include "Physics_Walkthrough_App.h"
#include "Physics\PhysicsRenderer.h"
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "Camera.h"
#include "Physics\SphereCollider.h"
#include "Physics\Spring.h"

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

	//Create scene
	m_scene = new Physics::PhysicScene();	

	//Create Debug physics renderer
	m_renderer = new Physics::PhysicsRenderer();

	//Add objects to scene
	/*for (int x = 0; x < 10; x++)
	{
		for (int y = 0; y < 1; y++)
		{
			for (int z = 0; z < 10; z++)
			{
				Physics::PhysicsObject *object = new Physics::PhysicsObject();
				object->SetPos(glm::vec3(x - 5.0f, y, z - 5.0f));
				object->SetCollider(new Physics::SphereCollider(0.3f));
				m_scene->AttachObject(object);
				m_renderer->GetRenderInfro(object)->color = glm::vec4(rand() % 255 / 255.0f, rand() % 255 / 255.0f, rand() % 255 / 255.0f, 0.5f);
			}
		}
	}*/

	/*Physics::PhysicsObject *left = new Physics::PhysicsObject();
	left->SetPos(glm::vec3(-5,1 , 0));
	left->SetCollider(new Physics::SphereCollider(1.0f));
	m_scene->AttachObject(left);

	Physics::PhysicsObject *right = new Physics::PhysicsObject();
	right->SetPos(glm::vec3(5, 1, 0));
	right->SetCollider(new Physics::SphereCollider(1.0f));
	m_scene->AttachObject(right);
	
	Physics::PhysicsObject *top = new Physics::PhysicsObject();
	top->SetPos(glm::vec3(0, 1, 5));
	top->SetCollider(new Physics::SphereCollider(1.0f));
	m_scene->AttachObject(top);

	Physics::PhysicsObject *bottom = new Physics::PhysicsObject();
	bottom->SetPos(glm::vec3(0, 1, -5));
	bottom->SetCollider(new Physics::SphereCollider(1.0f));
	m_scene->AttachObject(bottom);

	Physics::Spring *spring = new Physics::Spring(left, right, 7, 200, 1);
	m_scene->AttachConstraint(spring);

	Physics::Spring *spring2 = new Physics::Spring(left, bottom, 7, 200, 1);
	m_scene->AttachConstraint(spring2);


	Physics::Spring *spring3 = new Physics::Spring(bottom, bottom, 7, 200, 1);
	m_scene->AttachConstraint(spring3);*/

	

	const int maxX = 3;
	const int maxY = 3;
	const int maxZ = 3;
	Physics::PhysicsObject* blob[maxX][maxY][maxZ];

	for (int x = 0; x < maxX; x++)	//Do Height Last
	{
		for (int y = 0; y < maxY; y++)
		{
			for (int z = 0; z < maxZ; z++)
			{
				Physics::PhysicsObject *obj = new Physics::PhysicsObject();				
				obj->SetPos(glm::vec3(x, y, z));
				obj->SetCollider(new Physics::SphereCollider(0.3f));
				blob[x][y][z] = obj;
				m_scene->AttachObject(obj);
			}
		}
	}

	for (int x = 0; x < maxX; x++)
	{
		for (int y = 0; y < maxY; y++)
		{
			for (int z = 0; z < maxZ; z++)
			{
				if (y < 2)	//Up
				{
					Physics::Spring *spring = new Physics::Spring(blob[x][y][z], blob[x][y + 1][z], 1, 200, 1);
					m_scene->AttachConstraint(spring); 
				}
				if (x < 2)	//Right
				{
					Physics::Spring *spring = new Physics::Spring(blob[x][y][z], blob[x + 1][y][z], 1, 200, 1);
					m_scene->AttachConstraint(spring);
				}
				if (z < 2)	//Left
				{
					Physics::Spring *spring = new Physics::Spring(blob[x][y][z], blob[x][y][z + 1], 1, 200, 1);
					m_scene->AttachConstraint(spring);
				}
				if (z < 2 && x < 2)
				{
					Physics::Spring *spring = new Physics::Spring(blob[x][y][z], blob[x + 1][y][z + 1], 1.5, 200, 1);
					m_scene->AttachConstraint(spring);
				}
				if (z < 2 && x < 2)
				{
					Physics::Spring *spring = new Physics::Spring(blob[x][y][z + 1], blob[x + 1][y][z], 1.5, 200, 1);
					m_scene->AttachConstraint(spring);
				}
				if (z < 2 && y < 2)
				{
					Physics::Spring *spring = new Physics::Spring(blob[x][y][z], blob[x][y + 1][z + 1], 1.5, 200, 1);
					m_scene->AttachConstraint(spring);
				}
				if (z < 2 && y < 2)
				{
					Physics::Spring *spring = new Physics::Spring(blob[x][y + 1][z], blob[x][y][z + 1], 1.5, 200, 1);
					m_scene->AttachConstraint(spring);
				}
				if (x < 2 && y < 2)
				{
					Physics::Spring *spring = new Physics::Spring(blob[x][y + 1][z], blob[x + 1][y][z], 1.5, 200, 1);
					m_scene->AttachConstraint(spring);
				}
				if (x < 2 && y < 2)
				{
					Physics::Spring *spring = new Physics::Spring(blob[x][y][z], blob[x + 1][y + 1][z], 1.5, 200, 1);
					m_scene->AttachConstraint(spring);
				}
				if (x < 2 && y < 2 && z < 2)
				{
					Physics::Spring *spring = new Physics::Spring(blob[x][y][z], blob[x + 1][y + 1][z + 1], 1.5, 200, 1);
					m_scene->AttachConstraint(spring);
				}
				if (x < 2 && y < 2 && z < 2)
				{
					Physics::Spring *spring = new Physics::Spring(blob[x][y + 1][z], blob[x + 1][y][z + 1], 1.5, 200, 1);
					m_scene->AttachConstraint(spring);
				}
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

	float force = 10;
	//if (input->isKeyDown(aie::INPUT_KEY_LEFT))m_objectOne->ApplyForce(glm::vec3(-force, 0, 0));
	//if (input->isKeyDown(aie::INPUT_KEY_RIGHT))m_objectOne->ApplyForce(glm::vec3(force, 0, 0));
	//if (input->isKeyDown(aie::INPUT_KEY_UP))m_objectOne->ApplyForce(glm::vec3(0, force, 0));
	//if (input->isKeyDown(aie::INPUT_KEY_DOWN))m_objectOne->ApplyForce(glm::vec3(0, -force, 0));

	if (input->wasMouseButtonPressed(aie::INPUT_MOUSE_BUTTON_LEFT))
	{
		Physics::PhysicsObject *object = new Physics::PhysicsObject();
		object->SetPos(m_camera->GetPosition());
		object->SetVelocty(m_camera->GetFront() * 20);
		object->SetCollider(new Physics::SphereCollider(1.0f));
		object->SetMass(2.0f);
		m_scene->AttachObject(object);
		m_renderer->GetRenderInfro(object)->color = glm::vec4(rand() % 255 / 255.0f, rand() % 255 / 255.0f, rand() % 255 / 255.0f, 0.5f);
	}

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

