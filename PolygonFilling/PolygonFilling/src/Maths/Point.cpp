#include "Point.h"

float Point::getX()
{
	return m_x;
}

float Point::getY()
{
	return m_y;
}

Point::Point(float x, float y)
{
	m_x = x;
	m_y = y;
}
