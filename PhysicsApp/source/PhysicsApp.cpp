#include "PhysicsApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "gl_core_4_4.h"
#include <glm\ext.hpp>
#include <Gizmos.h>
#include "Sphere.h"
#include "Plane.h"
#include "Box.h"
#include "Aabb.h"
#include <random>

#define _USE_MATH_DEFINES
#include <math.h>

PhysicsApp::PhysicsApp()
{

}

PhysicsApp::~PhysicsApp()
{

}

bool PhysicsApp::startup()
{
	// increase the 2d line count to maximize the number of objects we can draw aie::Gizmos::create(255U, 255U, 65535U, 65535U);
	aie::Gizmos::create(255U, 255U, 65535U, 65535U);

	m_2dRenderer = new aie::Renderer2D();
	m_font = new aie::Font("./font/consolas.ttf", 32);

	m_physicsScene = new PhysicsScene();
	m_physicsScene->setGravity(glm::vec2(0, -10));
	m_physicsScene->setTimeStep(0.01f);

	Plane* plane1 = new Plane(glm::vec2(0, -1), 50);
	plane1->setKinematic(true);
	m_physicsScene->addActor(plane1);

	Plane* plane2 = new Plane(glm::vec2(0, 1), 50);
	plane2->setKinematic(true);
	m_physicsScene->addActor(plane2);

	Plane* plane3 = new Plane(glm::vec2(-1, 0), 80);
	plane3->setKinematic(true);
	m_physicsScene->addActor(plane3);

	Plane* plane4 = new Plane(glm::vec2(1, 0), 80);
	plane4->setKinematic(true);
	m_physicsScene->addActor(plane4);

	/*for (int y = 0; y < 5; y++)
	{
		for (int x = 0; x < 10; x++)
		{
			glm::vec2 position(-30 + 7.5f * x + y, -30 + 15 * y + x);

			glm::vec4 randomColor(rand() % 256 / 255.f, rand() % 256 / 255.f, rand() % 256 / 255.f, 1);

			if (x % 2 == 0)
			{
				Box* box = new Box(position, glm::vec2(-50, 0), 3, glm::vec2(3), randomColor);
				box->setRotation(glm::radians((float)(rand() % 360)));
				m_physicsScene->addActor(box);
			}
			else
			{
				Sphere* ball = new Sphere(position, glm::vec2(0, 0), 3, 3, randomColor);
				ball->setRotation(glm::radians((float)(rand() % 360)));
				ball->setKinematic(true);
				m_physicsScene->addActor(ball);
			}
		}
	}*/

	Sphere* ball1 = new Sphere(glm::vec2(-40, 0), glm::vec2(0), 1, 3, glm::vec4(0.5f, 0, 1, 1));
	ball1->setKinematic(true);
	m_physicsScene->addActor(ball1);

	Sphere* ball2 = new Sphere(glm::vec2(40, 0), glm::vec2(20, 0), 1, 3, glm::vec4(1, 1, 0, 1));
	m_physicsScene->addActor(ball2);

	return true;
}

void PhysicsApp::shutdown()
{

	delete m_font;
	delete m_2dRenderer;
}

void PhysicsApp::update(float deltaTime)
{
	// input example
	aie::Input* input = aie::Input::getInstance();

	aie::Gizmos::clear();

	m_physicsScene->update(deltaTime);
	m_physicsScene->updateGizmos();

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
	{
		quit();
	}
	if (input->isKeyDown(aie::INPUT_KEY_SPACE))
	{
		m_physicsScene->setGravity(glm::vec2(0, -9.81f));
	}
}

void PhysicsApp::draw()
{
	// wipe the screen to the background colour
	clearScreen();
	//glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// begin drawing sprites
	m_2dRenderer->begin();

	float aspectRatio = 16 / 9.f;
	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100, -100 / aspectRatio, 100 / aspectRatio, -1.0f, 1.0f));

	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// done drawing sprites
	m_2dRenderer->end();
}

