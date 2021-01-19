#pragma once
#include "../Maths/Polygon.h"
#include "../Maths/LCA.h"
#include <GLFW\glfw3.h>

struct Color {
	float r, g, b;
};

class Drawing {
private :
	GLFWwindow* m_window;
	LCA m_lca;
public:
	Drawing(GLFWwindow* window);
	void DrawPixel(int x, int y, Color c);
	void DrawLine(Vector a, Vector b, Color c);
	void DrawPolygon(Polygon p, Color c);
	void Fill(Polygon p, Color c);
};