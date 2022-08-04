#pragma once
#include <math.h>
#include <iostream>
/*
	Розрахунок відстані +
	Get Set +
	Розрахунок координат через створення зв'язку
*/
using std::ostream;
class Point {
private:
	float m_x, m_y;
public:
	Point();
	Point(float, float);
	
	float get_x();
	float get_y();
	void set_x(float);
	void set_y(float);

	bool operator == (const Point&);

	friend float calc_distance(const Point*, const Point*);
	friend ostream& operator << (ostream&, const Point&);
};
