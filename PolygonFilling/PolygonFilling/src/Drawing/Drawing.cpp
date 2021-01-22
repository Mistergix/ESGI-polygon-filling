#include "Drawing.h"
#include <map>
#include <algorithm>
#include <iostream>

struct Maillon {
	float ymax, xmin;
	float coeffinv;
};

struct MaillonLCA {
	float ymax, x;
	float coeffinv;
	float xmin;
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


struct VectorIndex {
	Vector v;
	int i;
};

bool sort(VectorIndex a, VectorIndex b) {
	if (a.v.getY() < b.v.getY()) {
		return true;
	}

	if (a.v.getY() > b.v.getY()) {
		return false;
	}

	return a.v.getX() < b.v.getX();
}

bool sortMaillon(Maillon a, Maillon b) {
	if (a.xmin < b.xmin) {
		return true;
	}

	if (a.xmin > b.xmin) {
		return false;
	}

	return a.coeffinv < b.coeffinv;
}

bool sortMaillonLCA(MaillonLCA a, MaillonLCA b) {
	return sortMaillon({a.ymax, a.xmin, a.coeffinv}, { b.ymax, b.xmin, b.coeffinv });
}

//LCA
void Drawing::Fill(Polygon p, Color c, GLubyte(*texture)[SCR_WIDTH][4])
{
	if (p.PointCount() <= 2) {
		return;
	}

	std::vector<Maillon> SI[SCR_HEIGHT];
	//creation du SI
	for (int y = 0; y < SCR_HEIGHT; y++) {
		SI[y] = std::vector<Maillon>();

		//std::sort(SI[y].begin(), SI[y].end(), sortMaillon);
	}

	std::vector<VectorIndex> orderedPoints;
	for (int i = 0; i < p.PointCount(); i++) {
		orderedPoints.push_back({ p.GetPoint(i), i });
	}

	std::sort(orderedPoints.begin(), orderedPoints.end(), sort);

	int MIN_Y = orderedPoints[0].v.getY();
	int MAX_Y = orderedPoints[orderedPoints.size() - 1].v.getY();

	std::vector<int> ys;

	for (int i = 0; i < orderedPoints.size(); i++)
	{
		Vector current = orderedPoints[i].v;
		Vector prev = p.GetPoint((orderedPoints[i].i - 1) < 0 ? p.PointCount() - 1 : orderedPoints[i].i - 1);
		Vector next = p.GetPoint((orderedPoints[i].i + 1) % p.PointCount());

		if (orderedPoints[i].i == 4) {
			int a = 0;
		}

		if (current.getY() < prev.getY()) {
			float xmin, ymax, ymin;

			ymax = prev.getY();
			ymin = current.getY();
			xmin = current.getX();
			float coeff = (current.getY() - prev.getY()) / (current.getX() - prev.getX());
			SI[(int)(current.getY())].push_back({ ymax, xmin, coeff == 0.0f ? 0.0f : 1 / coeff });
		}

		if (current.getY() < next.getY()) {
			float xmin, ymax, ymin;

			ymax = next.getY();
			ymin = current.getY();
			xmin = current.getX();
			float coeff = (current.getY() - next.getY()) / (current.getX() - next.getX());
			SI[(int)(current.getY())].push_back({ ymax, xmin, coeff == 0.0f ? 0.0f : 1 / coeff });
		}

		if (SI[(int)(current.getY())].size()) {
			ys.push_back((int)(current.getY()));
		}
	}
	
	for (int y = 0; y < SCR_HEIGHT; y++) {
		std::sort(SI[y].begin(), SI[y].end(), sortMaillon);
	}

	// SI CREE, ALGO LCA
	
	std::vector<MaillonLCA> lca;
	for (int y = MIN_Y; y <= MAX_Y; y++)
	{
		// RAJOUTER si y = ymin
		for (int i = 0; i < SI[y].size(); i++)
		{
			Maillon a = SI[y][i];
			float x = a.xmin; // ??
			lca.push_back({ a.ymax, x, a.coeffinv, a.xmin });
		}

		// RETIRER si y = ymax
		std::vector<int> indexesToRemove;
		if (lca.size() != 0) {
			for (int i = lca.size() - 1; i >= 0; i--)
			{
				if (lca[i].ymax == y) {
					indexesToRemove.push_back(i);
				}
			}
		}

		for (int i = 0; i < indexesToRemove.size(); i++)
		{
			int index = indexesToRemove[i];
			lca.erase(lca.begin() + index);
		}

		// trier
		std::sort(lca.begin(), lca.end(), sortMaillonLCA);
		
		// DESSIN
		int bibi = 0;
		if (lca.size() != 0) {
			while (bibi < lca.size() - 1) {
				if (bibi >= (lca.size() - 1)) {
					break;
				}
				DrawLine({ (lca[bibi].x), (float)y }, { lca[bibi + 1].x, (float)y }, c, texture);
				bibi += 2;
			}
		}

		// UPDATE LCA
		for (int i = 0; i < lca.size(); i++)
		{
			lca[i].x = (lca[i].x + lca[i].coeffinv);
		}

		continue;
	}
}
