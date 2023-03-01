#pragma once
class Point2D
{
public:
	Point2D();
	Point2D(int a, int b);
	//~Point2D();
	void setX(int num) { x = num; }
	void setY(int num) { y = num; }
	int getX() { return x; }
	int getY() { return y; }
private:
	int x, y;
};

