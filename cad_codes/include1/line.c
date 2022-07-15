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
{	//��ָ��Ϊ������ֱ��
	LineT l = (LineT)obj;
	Line(l->x1, l->y1, l->x2, l->y2);
}

bool InLine(double mx, double my, void *obj)
{	//�ж��Ƿ�ѡ��ֱ��
	LineT l = (LineT)obj;
	double slope; //ֱ��б��
	slope = (l->y2 - l->y1) / (l->x2 - l->x1);
	
	bool a, b;
	if(fabs(slope) < 100)
	{	//���б�ʽ�С
		if(l->x1 < l->x2)
			a = (l->x1 - 0.1 < mx && l->x2 + 0.1 > mx);
		else	//���Ⱥ�����������֮��
			a = (l->x2 - 0.1 < mx && l->x1 + 0.1 > mx);
		
		//����ֱ�߾���С��0.1
		b = (l->y1 + slope * (mx - l->x1) - 0.1*sqrt(1+slope*slope) < my && my < l->y1 + slope * (mx - l->x1) + 0.1*sqrt(1+slope*slope));
	}
	else
	{	//б��̫��
		if(l->x1 < l->x2)
			a = (l->x1 - 0.1 < mx && l->x2 + 0.1 > mx);
		else
			a = (l->x2 - 0.1 < mx && l->x1 + 0.1 > mx);
			
		if(l->y1 < l->y2)	//�ʵ�����ѡ�еķ�Χ
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
