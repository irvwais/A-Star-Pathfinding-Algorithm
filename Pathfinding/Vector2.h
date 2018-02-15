#pragma once
class Vector2
{
public:
	int x;
	int y;

	Vector2();
	Vector2(int x, int y);
	~Vector2();

	Vector2 operator+(const Vector2& v);
};

