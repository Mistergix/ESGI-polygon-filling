#include "Polygon.h"

Polygon::Polygon()
{
	std::vector<Point> vector;
	m_points = vector;
}

Polygon::Polygon(std::vector<Point> points)
{
	m_points = points;
}

//https://stackoverflow.com/questions/217578/how-can-i-determine-whether-a-2d-point-is-within-a-polygon
bool Polygon::IsInside(Point p)
{	 
	float minX = m_points[0].getX();
	float maxX = minX;
	float minY = m_points[0].getY();
	float maxY = minY;

	int count = m_points.size();

	// compute bounding box
	for (int i = 1; i < count; i++)
	{
		Point q = m_points[i];
		minX = std::min(q.getX(), minX);
		maxX = std::max(q.getX(), maxX);
		minY = std::min(q.getY(), minY);
		maxY = std::max(q.getY(), maxY);
	}

	if (p.getX() < minX || p.getX() > maxX || p.getY() < minY || p.getY() > maxY)
	{
		return false;
	}

	bool inside = false;
	for (int i = 0, j = count - 1; i < count; j = i++)
	{
		bool condition1 = (m_points[i].getY() > p.getY()) != (m_points[j].getY() > p.getY());
		bool condition2 = p.getX() < (m_points[j].getX() - m_points[i].getX()) * (p.getY() - m_points[i].getY()) / (m_points[j].getY() - m_points[i].getY()) + m_points[i].getX();
		if ( condition1 && condition2) {
			inside = !inside;
		}
	}

	return inside;
}
