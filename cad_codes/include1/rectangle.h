/*
 *rectangle.h
*/

#ifndef _RECTANGLE_H_
#define _RECTANGLE_H_

#include "genlib.h"
#include "graphics.h"
#include "extgraph.h"
#include "strlib.h"

struct rectangle{
    double x1, x2, y1, y2;
};

typedef struct rectangle *RectT; //�������ָ�� 

RectT NewRec();  //�½�һ���յľ���ָ�� 
void Rect(double x1, double y1, double l, double w);	//������ 
void Rect1(void *obj);	//��ָ��Ϊ��������ָ������ 
bool inBox(double x0, double y0, double x1, double x2, double y1, double y2);
bool InRec(double x, double y, void *obj); //�ж�(x,y)�Ƿ���ָ���Ӧ�ľ�����
void highlight_rect(void *obj, int judge);

#endif
