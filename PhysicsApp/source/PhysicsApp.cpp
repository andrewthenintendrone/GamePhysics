#include "PhysicsApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "gl_core_4_4.h"
#include <glm\ext.hpp>
#include <Gizmos.h>
#include "Sphere.h"
#include "Plane.h"
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
	m_diceTexture = new aie::Texture("./textures/dice_spritesheet.png");

	m_physicsScene = new PhysicsScene();
	m_physicsScene->setGravity(glm::vec2(0, -9.81f));
	m_physicsScene->setTimeStep(0.01f);

	for (int x = 0; x < 5; x++)
	{
		for (int y = 0; y < 5; y++)
		{
			Sphere* ball = new Sphere(glm::vec2(-20 + x * 10, -20 + y * 10), glm::vec2(15, 0), 1, 2, glm::vec4(1, 0, 0, 1));
			ball->setElasticity(0.8f);
			m_physicsScene->addActor(ball);
		}
	}

	// create walls
	Plane* wall1 = new Plane(glm::normalize(glm::vec2(-1, -1)), 40);
	Plane* wall2 = new Plane(glm::normalize(glm::vec2(-1, 1)), 40);
	Plane* wall3 = new Plane(glm::normalize(glm::vec2(1, -1)), 40);
	Plane* wall4 = new Plane(glm::normalize(glm::vec2(1, 1)), 40);

	m_physicsScene->addActor(wall1);
	m_physicsScene->addActor(wall2);
	m_physicsScene->addActor(wall3);
	m_physicsScene->addActor(wall4);

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
	//m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// done drawing sprites
	m_2dRenderer->end();
}
