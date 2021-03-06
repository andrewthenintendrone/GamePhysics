﻿#include "PhysicsApp.h"
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
	m_physicsScene->setGravity(glm::vec2(0, -100));
	m_physicsScene->setTimeStep(0.01f);

	Plane* plane1 = new Plane();
	plane1->setNormal(1, 2);
	plane1->setDistance(300);
	m_physicsScene->addActor(plane1);

	Plane* plane2 = new Plane();
	plane2->setNormal(1, -2);
	plane2->setDistance(300);
	m_physicsScene->addActor(plane2);
	
	int numBoxes = 16;

	for (int i = 0; i < numBoxes; i++)
	{
		Box* box = new Box();

		float theta = glm::radians(i * 360.0f / (float)numBoxes);

		float sn = std::sinf(theta);
		float cs = std::cosf(theta);

		glm::vec2 position(sn, -cs);
		position *= 200;
		position.x += getWindowWidth() * 0.5f;
		position.y += getWindowHeight() * 0.5f;

		box->setHeight(10);
		box->setWidth(10);
		box->setPosition(position);
		box->setMass(1);
		box->setKinematic(true);
		box->calculateMoment();
		m_physicsScene->addActor(box);
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

	glm::vec2 mousePos = glm::vec2(input->getMouseX(), input->getMouseY());

	if (input->wasMouseButtonPressed(0))
	{
		glm::vec4 randomColor(rand() % 256 / 255.0f, rand() % 256 / 255.0f, rand() % 256 / 255.0f, 1.0f);

		Sphere* ball = new Sphere();
		ball->setPosition(mousePos);
		ball->setMass(1);
		ball->setRadius(20);
		ball->setElasticity(0.3f);
		ball->setColor(randomColor);
		ball->calculateMoment();
		m_physicsScene->addActor(ball);
	}

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
	aie::Gizmos::draw2D(glm::ortho<float>(0, (float)getWindowWidth(), 0, (float)getWindowHeight(), -1.0f, 1.0f));

	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// done drawing sprites
	m_2dRenderer->end();
}

