#pragma once
#include "Vector.h"
#include <vector>

struct BoundingBox {
	float xMin, xMax, yMin, yMax;
};

class Polygon {
private:
	std::vector<Vector> m_points;

public:
	Polygon();
	Polygon(std::vector<Vector> points);
	bool IsInside(Vector pt);
	void Add(Vector pt);
	void Clear();
	std::vector<Vector> GetPoints();
	BoundingBox GetBoundingBox();
};