#pragma once
#include "Point.h"
#include <vector>

struct BoundingBox {
	float xMin, xMax, yMin, yMax;
};

class Polygon {
private:
	std::vector<Point> m_points;

public:
	Polygon();
	Polygon(std::vector<Point> points);
	bool IsInside(Point pt);
	void Add(Point pt);
	void Clear();
	std::vector<Point> GetPoints();
	BoundingBox GetBoundingBox();
};