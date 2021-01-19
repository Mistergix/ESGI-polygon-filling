#pragma once
#include "Polygon.h"

class Sutherland {
public:
	Polygon Clip(Polygon poly, Polygon window);
};