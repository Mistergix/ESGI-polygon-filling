#include "Drawing.h"

Drawing::Drawing(int w, int h)
{
	m_width = w;
	m_height = h;
	m_lca = LCA();
}

void Drawing::DrawPixel(int x, int y, Color c, GLubyte(*texture)[1000][4])
{
	texture[y][x][0] = c.r;
	texture[y][x][1] = c.g;
	texture[y][x][2] = c.b;
	texture[y][x][3] = c.a;
}

void Drawing::DrawLine(Vector a, Vector b, Color c)
{
}

void Drawing::DrawPolygon(Polygon p, Color c)
{
}

void Drawing::Fill(Polygon p, Color c)
{
	//m_lca.Fill(p);
	// faire quelque chose avec la couleur
}
