#pragma once

class AABBPoints
{
public:

	float left = 0;
	float right = 0;
	float top = 0;
	float bottom = 0;

	bool intersects(const AABBPoints& other);
};