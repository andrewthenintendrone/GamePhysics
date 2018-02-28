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
	//m_physicsScene->setGravity(glm::vec2(0, -10));
	m_physicsScene->setTimeStep(0.01f);

	std::vector<glm::vec2> points;

	for (int i = 0; i < 4; i++)
	{
		float theta = glm::radians(i * 360.0f / 4.0f);

		float sn = sinf(theta);
		float cs = cosf(theta);

		glm::vec2 point(sn, -cs);
		
		point *= 8;

		points.push_back(point);
	}

	phy::Polygon* p1 = new phy::Polygon(points);

	for (auto iter = points.begin(); iter != points.end(); iter++)
	{
		*iter *= 0.5f;
	}

	phy::Polygon* p2 = new phy::Polygon(points);

	p1->setPosition(glm::vec2(-10, 30));
	p2->setPosition(glm::vec2(50, 30));

	p1->setRotation(45.0f);
	p2->setRotation(45.0f);

	p1->setVelocity(glm::vec2(35, 0));
	p2->setVelocity(glm::vec2(-10, 0));

	p1->setAngularVelocity(15);
	p2->setAngularVelocity(45);

	p1->setLinearDrag(0);
	p2->setLinearDrag(0);

	p1->setAngularDrag(0);
	p2->setAngularDrag(0);

	p1->setElasticity(1);
	p2->setElasticity(1);

	p1->setColor(glm::vec4(1, 0, 0, 1));
	p2->setColor(glm::vec4(0, 0, 1, 1));

	m_physicsScene->addActor(p1);
	m_physicsScene->addActor(p2);

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

