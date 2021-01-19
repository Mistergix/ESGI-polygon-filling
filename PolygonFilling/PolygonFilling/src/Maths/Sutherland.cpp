#include "Sutherland.h"


bool Sutherland::Cut(Vector S, Vector Pj, Vector Fi, Vector Fiplus1)
{
	Mat2 A{ Pj.getX() - S.getX(), Fi.getX() - Fiplus1.getX(), Pj.getY() - S.getY(), Fi.getY() - Fiplus1.getY() };

	if (A.Determinant() == 0.0f) {
		// parallèle ou confondue
		return false;
	}

	Mat2 inv = m_maths.Inverse(A);
	Vector ts = m_maths.Product(inv, Vector{Fi.getX() - S.getX(), Fi.getY() - S.getY()});
	return 0 <=  ts.getX() && ts.getX() <= 1; // t in [0, 1], must be a segment
}

Vector Sutherland::Intersection(Vector S, Vector Pj, Vector Fi, Vector Fiplus1)
{
	Mat2 A{ Pj.getX() - S.getX(), Fi.getX() - Fiplus1.getX(), Pj.getY() - S.getY(), Fi.getY() - Fiplus1.getY() };
	Mat2 inv = m_maths.Inverse(A);
	Vector ts = m_maths.Product(inv, Vector{ Fi.getX() - S.getX(), Fi.getY() - S.getY() });
	Vector a = m_maths.Add(S, m_maths.Multiply(m_maths.Add(Pj, m_maths.Multiply(S, -1)), ts.getX()));
	Vector b = m_maths.Add(Fi, m_maths.Multiply(m_maths.Add(Fiplus1, m_maths.Multiply(Fi, -1)), ts.getY()));

	return a;
}

bool Sutherland::Visible(Vector S, Vector Fi, Vector Fiplus1)
{
	return false;
}

Sutherland::Sutherland()
{
	m_maths = Maths();
}

Polygon Sutherland::Clip(Polygon poly, Polygon window)
{

	return Polygon();
}
