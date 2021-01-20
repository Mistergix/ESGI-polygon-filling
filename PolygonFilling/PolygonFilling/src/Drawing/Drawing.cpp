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
/*https://stackoverflow.com/questions/10060046/drawing-lines-with-bresenhams-line-algorithm#answer-16405254*/
void Drawing::DrawLine(Vector a, Vector b, Color c, GLubyte(*texture)[SCR_WIDTH][4])
{
	int x1, y1, x2, y2;
	x1 = (int)a.getX();
	x2 = (int)b.getX();
	y1 = (int)a.getY();
	y2 = (int)b.getY();

	int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
	dx = x2 - x1;
	dy = y2 - y1;
	dx1 = fabs(dx);
	dy1 = fabs(dy);
	px = 2 * dy1 - dx1;
	py = 2 * dx1 - dy1;
	if (dy1 <= dx1) {
		if (dx >= 0) {
			x = x1;
			y = y1;
			xe = x2;
		}
		else {
			x = x2;
			y = y2;
			xe = x1;
		}

		DrawPixel(x, y, c, texture);
		for (i = 0; x < xe; i++)
		{
			x++;
			if (px < 0) {
				px = px + 2 * dy1;
			}
			else {
				if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
				{
					y++;
				}
				else
				{
					y--;
				}
				px = px + 2 * (dy1 - dx1);
			}
			DrawPixel(x, y, c, texture);
		}
	}
	else {
		if (dy >= 0) {
			x = x1;
			y = y1;
			ye = x2;
		}
		else {
			x = x2;
			y = y2;
			ye = x1;
		}

		DrawPixel(x, y, c, texture);
		for (i = 0; y < ye; i++)
		{
			y++;
			if (py <= 0) {
				py = py + 2 * dx1;
			}
			else {
				if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
				{
					x++;
				}
				else
				{
					x--;
				}
				py = py + 2 * (dx1 - dy1);
			}
			DrawPixel(x, y, c, texture);
		}
	}
	/*
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
	*/
	
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
