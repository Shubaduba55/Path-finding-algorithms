#include "Point.h"

Point::Point() :m_x(0), m_y(0) {}
Point::Point(float x, float y) : m_x(x), m_y(y) {}

float Point::get_x() { return m_x; }
float Point::get_y() { return m_y; }
void Point::set_x(float x) { 
	if (x < 0 || x > 100) throw "x is out of range [0;100]";
	m_x = x; 
	return; 
}//Error: out of range (0, 100)

void Point::set_y(float y) { 
	if (y < 0 || y > 100) throw "y is out of range [0;100]";
	m_y = y; 
	return;
}//Error: out of range (0, 100)

bool Point::operator==(const Point& other)
{	
	if (m_x == other.m_x && m_y == other.m_y) return true;
	else return false;
}

float calc_distance(const Point* A, const Point* B)
{
	float min_x = A->m_x - B->m_x;
	float min_y = A->m_y - B->m_y;
	float distance = sqrt(pow(min_x, 2) + pow(min_y, 2));
	return distance;
}

ostream& operator<<(ostream& out, const Point& point)
{
	out << "(" << point.m_x << "; " << point.m_y << ")";
	return out;
}
