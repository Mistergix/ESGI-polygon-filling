#include "Sutherland.h"


bool Sutherland::Cut(Vector S, Vector Pj, Vector Fi, Vector Fiplus1)
{
	Mat2 A{ Pj.getX() - S.getX(), Fi.getX() - Fiplus1.getX(), Pj.getY() - S.getY(), Fi.getY() - Fiplus1.getY() };

	if (A.Determinant() == 0.0f) {
		// parall�le ou confondue
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
	// Vector b = m_maths.Add(Fi, m_maths.Multiply(m_maths.Add(Fiplus1, m_maths.Multiply(Fi, -1)), ts.getY())); // a and b are equal

	return a;
}

bool Sutherland::Visible(Vector S, Vector Fi, Vector Fiplus1, Polygon windowPolygon)
{
	Vector normal = windowPolygon.GetNormalInterior(Fi, Fiplus1);
	Vector a{ S.getX() - Fi.getX(), S.getY() - Fi.getY() };

	float scalar = m_maths.Scalar(normal, a);

	return scalar >= 0.0f;
}

Sutherland::Sutherland()
{
	m_maths = Maths();
}

Polygon Sutherland::Clip(Polygon& poly, Polygon& window)
{
	int N1 = poly.PointCount();
	int N3 = window.PointCount();
	Vector F;
	Vector S;
	Polygon PL = poly;
	for (int i = 0; i < N3 - 1; i++)
	{
		int N2 = 0;
		Polygon PS = Polygon();
		for (int j = 0; j < N1; j++)	
		{
			if (j == 0) {
				F = PL.GetPoint(j);
			}
			else
			{
				if (Cut(S , PL.GetPoint(j), window.GetPoint(i), window.GetPoint(i+1))) {
					Vector I = Intersection(S, PL.GetPoint(j), window.GetPoint(i), window.GetPoint(i + 1));
					PS.Add(I);
					N2++;
				}
			}
			S = PL.GetPoint(j);
			if (Visible(S, window.GetPoint(i), window.GetPoint(i + 1), window)) {
				PS.Add(S);
				N2++;
			}
		}
		if (N2 > 0) {
			if (Cut(S, F, window.GetPoint(i), window.GetPoint(i + 1))) {
				Vector I = Intersection(S, F, window.GetPoint(i), window.GetPoint(i + 1));
				PS.Add(I);
				N2++;
			}
			PL = PS;
			N1 = N2;
		}
	}
	return PL;
}
 