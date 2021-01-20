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

	if (x0 == x1) {
		// VERTICAL LINE
		int begin = 0, end = 0;
		if (y0 < y1) {
			begin = y0;
			end = y1;
		}
		else
		{
			begin = y1;
			end = y0;
		}

		for (int y = begin; y <= end; y++)
		{
			DrawPixel(x0, y, c, texture);
		}
		
		return;
	}

	float m = (float)(y1 - y0) / (x1 - x0);
	float ordonnee = y0 - m * x0;
	int begin, end;
	if (x0 < x1) {
		begin = x0;
		end = x1;
	}
	else {
		begin = x1;
		end = x0;
	}

	for (int x = begin; x <= end; x++)
	{
		int y = round(m * x + ordonnee);
		DrawPixel(x, y, c, texture);
	}

	
}

void Drawing::DrawPolygon(Polygon p, Color c, GLubyte(*texture)[SCR_WIDTH][4])
{
	if (p.PointCount() < 2) { return; }

	for (int i = 0; i <= p.PointCount(); i++)
	{
		int curr = i % p.PointCount();
		int next = (i + 1) % p.PointCount();
		DrawLine(p.GetPoint(curr), p.GetPoint(next), c, texture);
	}
}

void Drawing::Fill(Polygon p, Color c)
{
	//m_lca.Fill(p);
	// faire quelque chose avec la couleur
}
