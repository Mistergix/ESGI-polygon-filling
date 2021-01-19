#pragma once
#include "Polygon.h"

class Sutherland {
private:
	
	Point Intersection(Point S, Point Pj, Point Fi, Point Fiplus1);
	Point IntersectionDroiteDroite(Point S, Point Pj, Point Fi, Point Fiplus1);
	bool Visible(Point S, Point Fi, Point Fiplus1);
public:
	Polygon Clip(Polygon poly, Polygon window);
	bool Cut(Point S, Point Pj, Point Fi, Point Fiplus1);
};