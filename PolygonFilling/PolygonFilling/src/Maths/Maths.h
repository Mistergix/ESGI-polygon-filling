#pragma once
#include <math.h>

struct Vector { 
	float x, y; 
	float Magnitude() { return sqrt(x * x + y * y); }
};

struct Mat2 {
	float a, b;
	float c, d;

	float Determinant() { return a * d - b * c; }
};

class Maths {
public:
	bool Colinear(Vector a, Vector b) {
		return (a.x * b.y - a.y * b.x) == 0;
	}

	Vector Product(Mat2 m, Vector v) {
		float M = m.a * v.x + m.b * v.y;
		float N = m.c * v.x + m.d * v.y;

		return Vector{ M, N };
	}

	Mat2 Inverse(Mat2 m) {
		float det = m.Determinant();
		return Mat2{ m.d / det, -m.b / det, -m.c / det, m.a / det };
	}
};