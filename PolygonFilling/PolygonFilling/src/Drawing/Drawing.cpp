#include "Drawing.h"

Drawing::Drawing(int w, int h)
{
	m_width = w;
	m_height = h;
	m_lca = LCA();
}

void Drawing::DrawPixel(int x, int y, Color c, GLubyte(*texture)[SCR_WIDTH][4])
{
	texture[y][x][0] = c.r;
	texture[y][x][1] = c.g;
	texture[y][x][2] = c.b;
	texture[y][x][3] = c.a;
}

void Drawing::DrawLine(Vector a, Vector b, Color c, GLubyte(*texture)[SCR_WIDTH][4])
{
	int x0, y0, x1, y1;
	x0 = (int)a.getX();
	y0 = (int)a.getY();
	x1 = (int)b.getX();
	y1 = (int)b.getY();

	if (y1 < y0) {
		int tmp = y0;
		y0 = y1;
		y1 = tmp;
	}

	if (x0 == x1) {
		// VERTICAL LINE
		for (int y = y0; y <= y1; y++)
		{
			DrawPixel(x0, y, c, texture);
		}
		return;
	}

	if (x1 < x0) {
		int tmp = x0;
		x0 = x1;
		x1 = tmp;
	}

	float m = (float)(y1 - y0) / (x1 - x0);
	float ordonnee = y0 - m * x0;
	for (int x = x0; x <= x1; x++)
	{
		int y = round(m * x + ordonnee);
		DrawPixel(x, y, c, texture);
	}
}

void Drawing::DrawPolygon(Polygon p, Color c)
{
}

void Drawing::Fill(Polygon p, Color c)
{
	//m_lca.Fill(p);
	// faire quelque chose avec la couleur
}
