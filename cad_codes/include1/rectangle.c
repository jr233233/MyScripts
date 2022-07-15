#include "rectangle.h"

RectT NewRec()	//新建一个空的矩形指针
{
	RectT r = New(RectT);
	return r;
}

void Rect(double x1, double y1, double x2, double y2)
{
	MovePen(x1, y1);
	DrawLine(x2-x1, 0);
	DrawLine(0, y2-y1);
	DrawLine(x1-x2, 0);
	DrawLine(0, y1-y2);
}

void Rect1(void *obj)
{
	RectT r = (RectT)obj;
	Rect(r->x1, r->y1, r->x2, r->y2);
}

bool inBox(double x0, double y0, double x1, double x2, double y1, double y2)
{
	bool a, b;
	if(x2>x1)
		a = (x0 >= x1 && x0 <= x2);
	else
		a = (x0 <= x1 && x0 >= x2);
	
	if(y2>y1)
		b = (y0 >= y1 && y0 <= y2);
	else
		b = (y0 <= y1 && y0 >= y2);
	
	return (a&&b);
}

bool InRec(double x, double y, void *obj)
{
	RectT r = (RectT)obj;
	return inBox(x,y, r->x1, r->x2, r->y1, r->y2);
}

void highlight_rect(void *obj, int judge)
{	//让矩形突出显示
	if(judge == 1)
		SetPenColor("Red");
	else
		SetPenColor("Black");
	RectT r = (RectT)obj;
	Rect1(r);
	SetPenColor("Black");
}
