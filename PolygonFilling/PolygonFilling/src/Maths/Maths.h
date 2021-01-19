#pragma once
#include "Vector.h"

struct Mat2 {
	float a, b;
	float c, d;

	float Determinant() { return a * d - b * c; }
};

class Maths {
public:
	bool Colinear(Vector a, Vector b) {
		return (a.getX() * b.getY() - a.getY() * b.getX()) == 0;
	}

	Vector Product(Mat2 m, Vector v) {
		float M = m.a * v.getX() + m.b * v.getY();
		float N = m.c * v.getX() + m.d * v.getY();

		return Vector{ M, N };
	}

	Mat2 Inverse(Mat2 m) {
		float det = m.Determinant();
		return Mat2{ m.d / det, -m.b / det, -m.c / det, m.a / det };
	}

	Vector Add(Vector a, Vector b) {
		return Vector(a.getX() + b.getX(), a.getY() + b.getY());
	}

	Vector Multiply(Vector a, float b) {
		return Vector(a.getX() * b, a.getY() * b);
	}
};