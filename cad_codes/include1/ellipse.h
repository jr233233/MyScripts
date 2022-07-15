/*
 *ellipse.h
 */

#ifndef _ELLIPSE_H_
#define _ELLIPSE_H_

#include "genlib.h"
#include "graphics.h"
#include "extgraph.h"
#include "strlib.h"

struct ellipse
{
    double x, y;    //Ô²ÐÄ×ø±ê
    double a, b;    //°ë³¤¶ÌÖá
};
typedef struct ellipse *ElliT;

ElliT NewEllipse();
void Elli(double x1, double y1, double x2, double y2);
void Elli1(void *obj);
bool InElli(double mx, double my, void *obj);
bool inBox(double x0, double y0, double x1, double x2, double y1, double y2);
void highlight_ell(void *obj, int judge);

#endif
