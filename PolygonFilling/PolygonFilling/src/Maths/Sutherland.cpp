#include "Sutherland.h"
#include "../Maths/Maths.h"

bool Sutherland::Cut(Point S, Point Pj, Point Fi, Point Fiplus1)
{
	Maths m_maths = Maths();
	Mat2 A{ Pj.getX() - S.getX(), Fi.getX() - Fiplus1.getX(), Pj.getY() - S.getY(), Fi.getY() - Fiplus1.getY() };

	if (A.Determinant() == 0.0f) {
		// parallèle ou confondue
		return false;
	}

	Mat2 inv = m_maths.Inverse(A);
	Vector ts = m_maths.Product(inv, Vector{Fi.getX() - S.getX(), Fi.getY() - S.getY()});
	return 0 <=  ts.x && ts.x <= 1; // t in [0, 1], must be a segment
}

Point Sutherland::Intersection(Point S, Point Pj, Point Fi, Point Fiplus1)
{
	return Point();
}

Point Sutherland::IntersectionDroiteDroite(Point S, Point Pj, Point Fi, Point Fiplus1)
{

	return Point();
}

bool Sutherland::Visible(Point S, Point Fi, Point Fiplus1)
{
	return false;
}

Polygon Sutherland::Clip(Polygon poly, Polygon window)
{

	return Polygon();
}
