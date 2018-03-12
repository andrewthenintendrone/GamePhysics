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
	m_physicsScene->setGravity(glm::vec2(0, -20));
	m_physicsScene->setTimeStep(0.01f);

	Plane* plane1 = new Plane();
	plane1->setNormal(1, 2);
	plane1->setDistance(-50);
	m_physicsScene->addActor(plane1);

	Plane* plane2 = new Plane();
	plane2->setNormal(-1, 2);
	plane2->setDistance(-50);
	m_physicsScene->addActor(plane2);

	// spawn balls
	for (int x = 0; x < 10; x++)
	{
		for (int y = 0; y < 10; y++)
		{
			glm::vec2 position(-50 + x * 12.5f, y * 10);
			glm::vec4 randomColor(rand() % 256 / 255.0f, rand() % 256 / 255.0f, rand() % 256 / 255.0f, 1);

			float mass;
			glm::vec4 color;

			if (x % 2 == 0 ^ y % 2 == 0)
			{
				color = glm::vec4(0.5f, 0, 1, 1);
				mass = 1;
			}
			else
			{
				color = glm::vec4(1, 1, 0, 1);
				mass = 10;
			}

			Sphere* ball = new Sphere();
			ball->setPosition(position);
			ball->setVelocity(glm::vec2(20, 5));
			ball->setRadius(4);
			ball->setElasticity(0.3f);
			ball->setMass(mass);
			ball->setColor(randomColor);
			ball->calculateMoment();
			m_physicsScene->addActor(ball);
		}
	}


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
	m_physicsScene->draw();

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
	//m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// done drawing sprites
	m_2dRenderer->end();
}

