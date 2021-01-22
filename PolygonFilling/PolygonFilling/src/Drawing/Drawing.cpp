#include "Drawing.h"
#include <map>
#include <algorithm>

struct Maillon {
	int ymax, xmin;
	float coeffinv;
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



//LCA
void Drawing::Fill(Polygon p, Color c, GLubyte(*texture)[SCR_WIDTH][4])
{
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

	for (int i = 0; i < orderedPoints.size(); i++)
	{
		Vector current = orderedPoints[i].v;
		Vector prev = p.GetPoint((orderedPoints[i].i - 1) < 0 ? p.PointCount() - 1 : orderedPoints[i].i - 1);
		Vector next = p.GetPoint((orderedPoints[i].i + 1) % p.PointCount());

		if (orderedPoints[i].i == 4) {
			int a = 0;
		}

		if (current.getY() < prev.getY()) {
			int xmin, ymax, ymin;

			ymax = prev.getY();
			ymin = current.getY();
			xmin = current.getX();
			float coeff = (current.getY() - prev.getY()) / (current.getX() - prev.getX());
			SI[(int)(current.getY())].push_back({ ymax, xmin, coeff == 0.0f ? 0.0f : 1 / coeff });
		}

		if (current.getY() < next.getY()) {
			int xmin, ymax, ymin;

			ymax = next.getY();
			ymin = current.getY();
			xmin = current.getX();
			float coeff = (current.getY() - next.getY()) / (current.getX() - next.getX());
			SI[(int)(current.getY())].push_back({ ymax, xmin, coeff == 0.0f ? 0.0f : 1 / coeff });
		}
	}
	
	return;


	/*




	

	std::vector<Maillon> lcas;


	SI.insert((int)(orderedPoints[0].v.getY()), lcas);


	//creation du SI
	for (int i = 0; i < p.PointCount() - 1; i++) 
	{
		Vector a = p.GetPoint(i);
		Vector b = p.GetPoint(i + 1);

		int xmin, ymax, ymin;

		if (a.getY() < b.getY()) {
			ymax = b.getY();
			ymin = a.getY();

			xmin = a.getX();
		}
		else {
			ymax = a.getY();
			ymin = b.getY();

			xmin = b.getX();
		}

		if (ymax > MAX_Y) MAX_Y = ymax;
		if (ymin < MIN_Y) MIN_Y = ymin;

		float coeff = (a.getY() - b.getY()) /( a.getX() - b.getX());
		
		Maillon lca;
		lca.xmin = xmin;
		lca.ymax = ymax;
		lca.coeff = coeff == 0.0f ? 0.0f : 1 / coeff;
		coord.insert({ ymin, lca });

	}

	return;


	Maillon current = coord.at(MIN_Y);

	//liste triée, application LCA
	for (int y = MIN_Y+1; y < MAX_Y; y++) {
		int x1, x2;
		
		if (coord.find(y) == coord.end())//pas trouvé
		{
			x1 = current.xmin + current.coeff;
		}

		else { //trouvé
			current = coord.at(y);
			x1 = current.xmin;
		}

		for (int i = y; i < MAX_Y; i++) {
			if (coord.find(i) == coord.end()) {
				break;
			}
			else {
				x2 = coord.at(i).xmin + coord.at(i).coeff;
			}
		}

		DrawLine({ (float)x1, (float)y }, { (float)x2, (float)y }, c, texture);
	}*/
}
