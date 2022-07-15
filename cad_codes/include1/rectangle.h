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

typedef struct rectangle *RectT; //定义矩形指针 

RectT NewRec();  //新建一个空的矩形指针 
void Rect(double x1, double y1, double l, double w);	//画矩形 
void Rect1(void *obj);	//以指针为参数画出指定矩形 
bool inBox(double x0, double y0, double x1, double x2, double y1, double y2);
bool InRec(double x, double y, void *obj); //判断(x,y)是否在指针对应的矩形内
void highlight_rect(void *obj, int judge);

#endif
