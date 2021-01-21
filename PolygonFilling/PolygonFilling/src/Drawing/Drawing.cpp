#include "Drawing.h"

struct LCA {
	int ymax, xmin;
	float coeff;
};

Drawing::Drawing(int w, int h)
{
	m_width = w;
	m_height = h;
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
			ye = y2;
		}
		else {
			x = x2;
			y = y2;
			ye = y1;
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

//LCA
void Drawing::Fill(Polygon p, Color c, GLubyte(*texture)[SCR_WIDTH][4])
{
	std::vector<LCA> lca;
	
	for (int i = 0; i < p.PointCount() - 1; i++) 
	{
		Vector a = p.GetPoint(i);
		Vector b = p.GetPoint(i + 1);
		int ymax = (a.getY() < b.getY() ? b.getY() : a.getY());
		int xmin = (a.getX() < b.getX() ? a.getX() : b.getX());
		
		float coeff = (b.getY() - a.getY())/(b.getX() - a.getX());
		coeff = 1 / coeff;
		LCA tmp;
		tmp.ymax = ymax;
		tmp.xmin = xmin;
		tmp.coeff = coeff;

		lca.push_back(tmp);
	}
}
