#include "Drawing.h"
#include <GLFW\glfw3.h>
#include "../Maths/LCA.h"
#include "Texture.h"

Drawing::Drawing(GLFWwindow* window)
{
	m_window = window;
	m_lca = LCA();
}

void Drawing::DrawPixel(int x, int y, Color c) 
{
	
}

void Drawing::DrawLine(Point a, Point b, Color c) 
{

}

void Drawing::DrawPolygon(Polygon p, Color c)
{
	glColor3f(c);
	glPolygonMode(GL_FRONT, GL_LINE);
}

void Drawing::Fill(Polygon p, Color c)
{
	m_lca.Fill(p);
	// faire quelque chose avec la couleur
}
