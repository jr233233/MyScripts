/*
 *line.h
 */

#ifndef _LINE_H_
#define _LINE_H_

#include "genlib.h"
#include "graphics.h"
#include "extgraph.h"
#include "strlib.h"

struct line{
    double x1, x2, y1, y2;
};
typedef struct line *LineT;

LineT NewLine();
void Line(double x1, double y1, double x2, double y2);
void Line1(void *obj);
bool InLine(double mx, double my, void *obj);
void highlight_line(void *obj, int judge);

#endif

