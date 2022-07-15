#include "ellipse.h"

ElliT NewEllipse()
{
    ElliT e = New(ElliT);
    return e;
}

void Elli(double x, double y, double a, double b)
{   //��������Ϊ(x,y)���볤�ᣬ�����Ϊa��b����Բ
    MovePen(x + a, y);
    DrawEllipticalArc(a, b, 0.0, 360.0);
}

void Elli1(void *obj)
{   //����Բָ��Ϊ��������Բ
    ElliT e = (ElliT)obj;
    Elli(e->x, e->y, e->a, e->b);
}

bool inBox1(double x0, double y0, double x1, double x2, double y1, double y2)
{
    bool a, b;
    if (x2 > x1)
        a = (x0 >= x1 && x0 <= x2);
    else
        a = (x0 <= x1 && x0 >= x2);

    if (y2 > y1)
        b = (y0 >= y1 && y0 <= y2);
    else
        b = (y0 <= y1 && y0 >= y2);

    return (a && b);
}

bool InElli(double mx, double my, void *obj)
{
    ElliT e = (ElliT)obj;
    return inBox1(mx,my, e->x-e->a, e->x+e->a, e->y-e->b, e->y+e->b);
}

void highlight_ell(void *obj, int judge)
{   //����Բͻ����ʾ
    if (judge == 1)
        SetPenColor("Red");
    else
        SetPenColor("Black");

    Elli1(obj);
    SetPenColor("Black");
}

