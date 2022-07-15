#include "line.h"
#include <math.h>

LineT NewLine()
{
	LineT l = New(LineT);
	return l;
}

void Line(double x1, double y1, double x2, double y2)
{
	MovePen(x1, y1);
	DrawLine(x2 - x1, y2 - y1);
}

void Line1(void *obj)
{	//以指针为参数画直线
	LineT l = (LineT)obj;
	Line(l->x1, l->y1, l->x2, l->y2);
}

bool InLine(double mx, double my, void *obj)
{	//判断是否选中直线
	LineT l = (LineT)obj;
	double slope; //直线斜率
	slope = (l->y2 - l->y1) / (l->x2 - l->x1);
	
	bool a, b;
	if(fabs(slope) < 100)
	{	//如果斜率较小
		if(l->x1 < l->x2)
			a = (l->x1 - 0.1 < mx && l->x2 + 0.1 > mx);
		else	//首先横坐标再两点之间
			a = (l->x2 - 0.1 < mx && l->x1 + 0.1 > mx);
		
		//距离直线距离小于0.1
		b = (l->y1 + slope * (mx - l->x1) - 0.1*sqrt(1+slope*slope) < my && my < l->y1 + slope * (mx - l->x1) + 0.1*sqrt(1+slope*slope));
	}
	else
	{	//斜率太大
		if(l->x1 < l->x2)
			a = (l->x1 - 0.1 < mx && l->x2 + 0.1 > mx);
		else
			a = (l->x2 - 0.1 < mx && l->x1 + 0.1 > mx);
			
		if(l->y1 < l->y2)	//适当扩大选中的范围
			b = (l->y1 - 0.1 < my && l->y2 + 0.1 > my);
		else
			b = (l->y2 - 0.1 < my && l->y1 + 0.1 > my);
	}
	
	return a&&b;
}

void highlight_line(void *obj, int judge)
{
	if (judge == 1)
		SetPenColor("Red");
	else
		SetPenColor("Black");

	Line1(obj);
	SetPenColor("Black");
}
