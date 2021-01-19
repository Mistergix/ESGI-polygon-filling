#pragma once
#include "Point.h"
#include <vector>

class Polygon {
private:
	std::vector<Point> m_points;

public:
	Polygon();
	Polygon(std::vector<Point> points);
	bool IsInside(Point pt);
};