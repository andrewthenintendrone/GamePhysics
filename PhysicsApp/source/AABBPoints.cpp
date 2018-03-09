#include "AABBPoints.h"
#include "Projection.h"

bool AABBPoints::intersects(const AABBPoints& other)
{
	Projection x1 = Projection(left, right);
	Projection x2 = Projection(other.left, other.right);

	Projection y1 = Projection(bottom, top);
	Projection y2 = Projection(other.bottom, other.top);

	return (x1.contains(x2) || x1.overlaps(x2) && y1.contains(y2) || y1.overlaps(y2));
}
