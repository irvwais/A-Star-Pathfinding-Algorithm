#include "Vector2.h"

Vector2::Vector2()
	: Vector2(0, 0)
{
}

Vector2::Vector2(int x, int y)
	: x(x), y(y)
{
}


Vector2::~Vector2()
{
}

Vector2 Vector2::operator+(const Vector2 &v )
{
	return Vector2(x + v.x, y + v.y);
}
