#include "PhysicsApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "gl_core_4_4.h"
#include <glm\ext.hpp>
#include <Gizmos.h>
#include "Sphere.h"
#include "Plane.h"
#include "Polygon.h"
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

	for (int x = 0; x < 3; x++)
	{
		for (int y = 0; y < 3; y++)
		{
			glm::vec2 position(-50 + 50 * x, -20 + 20 * y);
			glm::vec4 randomColor(rand() % 256 / 255.0f, rand() % 256 / 255.0f, rand() % 256 / 255.0f, 1.0f);

			if (x % 2 == 0 ^ y % 2 == 0)
			{
				Sphere* sphere = new Sphere(position, glm::vec2(0), 1, 4, randomColor);
				sphere->setAngularVelocity(rand() % 360);
				m_physicsScene->addActor(sphere);
			}
			else
			{
				phy::Polygon* polygon = new phy::Polygon(5, 8);
				polygon->setPosition(position);
				polygon->setRotation(rand() % 360);
				polygon->setAngularVelocity(rand() % 360);
				polygon->setColor(randomColor);

				m_physicsScene->addActor(polygon);
			}
		}
	}

	Plane* plane1 = new Plane(glm::vec2(0, -1), 40);
	m_physicsScene->addActor(plane1);

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

