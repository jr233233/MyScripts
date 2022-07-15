#include "genlib.h"
#include "graphics.h"
#include "extgraph.h"
#include "strlib.h"

#define toolbar_len 0.7	   //工具栏的宽度
#define instruction_size 13 //说明文字的大小

void draw_box(double len, double wid)
{	//画矩形
	DrawLine(len, 0);
	DrawLine(0, wid);
	DrawLine(-len, 0);
	DrawLine(0, -wid);
}

void draw_circle(double r)
{
	MovePen(GetCurrentX() + r, GetCurrentY());
	DrawArc(r, 0, 360);
	MovePen(GetCurrentX() - r, 0);
}

bool in_box(double x0, double y0, double x1, double x2, double y1, double y2)
{
	return (((x0 >= x1 && x0 <= x2) || (x0 <= x1 && x0 >= x2)) && (y0 >= y1 && y0 <= y2) || (y0 <= y1 && y0 >= y2));
}

void draw_boundary()
{	//画边界线
	MovePen(0.1, 0.1);
	draw_box(GetWindowWidth() - 0.2, GetWindowHeight() - toolbar_len - 0.1);
}

void draw_toolbar(int order)
{ //画工具栏，并给order对应的栏突出显示(变红)
	SetEraseMode(FALSE);
	MovePen(GetWindowWidth() / 2 - 2 * toolbar_len, GetWindowHeight());
	DrawLine(0, -toolbar_len);
	int i;
	for (i = 0; i < 5; i++)
	{
		DrawLine(toolbar_len, 0);
		DrawLine(0, toolbar_len);
		DrawLine(-toolbar_len, 0);
		MovePen(GetCurrentX() + toolbar_len, GetCurrentY() - toolbar_len);
	}//画好了5个方框

	MovePen(GetWindowWidth() / 2 - 9 * toolbar_len,
			GetWindowHeight() - 0.55 * toolbar_len);
	DrawTextString("请在边框区域内画图");	//写提示语句

	MovePen(GetWindowWidth() / 2 - 4 * toolbar_len,
			GetWindowHeight() - 0.55 * toolbar_len);
	DrawTextString("工具栏");

	MovePen(GetWindowWidth() / 2 - 1.8 * toolbar_len,
			GetWindowHeight() - 0.5 * toolbar_len);
	DrawLine(0.5 * toolbar_len, 0);

	MovePen(GetWindowWidth() / 2 - 0.8 * toolbar_len,
			GetWindowHeight() - 0.6 * toolbar_len);
	draw_box(0.5 * toolbar_len, 0.2 * toolbar_len);

	MovePen(GetWindowWidth() / 2 + 0.5 * toolbar_len,
			GetWindowHeight() - 0.5 * toolbar_len);
	draw_circle(0.3 * toolbar_len);

	MovePen(GetWindowWidth() / 2 + 1.15 * toolbar_len,
			GetWindowHeight() - 0.6 * toolbar_len);
	SetEraseMode(FALSE); /*取消擦除模式*/
	DrawTextString("文本");

	MovePen(GetWindowWidth() / 2 + 2.15 * toolbar_len,
			GetWindowHeight() - 0.6 * toolbar_len);
	SetEraseMode(FALSE); /*取消擦除模式*/
	DrawTextString("帮助");

	draw_boundary();

	MovePen(GetWindowWidth() / 2, GetWindowHeight());

	if (order == 0)
		return;
	else
	{ //突出显示指定栏，1对应直线，2对应矩形，3对应椭圆，4对应文本
		MovePen(GetWindowWidth() / 2 + (order - 3) * toolbar_len, GetWindowHeight());
		SetPenColor("Red");
		DrawLine(toolbar_len, 0);
		DrawLine(0, -toolbar_len);
		DrawLine(-toolbar_len, 0);
		DrawLine(0, toolbar_len);
		SetPenColor("Black");
	}
}

int choice(double x, double y) //判断(x,y)位于哪一个工具栏
{
	if (in_box(x, y, GetWindowWidth() / 2 - 2 * toolbar_len, GetWindowWidth() / 2 - toolbar_len,
			   GetWindowHeight() - toolbar_len, GetWindowHeight()))
		return 1; //直线
	else if (in_box(x, y, GetWindowWidth() / 2 - toolbar_len, GetWindowWidth() / 2,
					GetWindowHeight() - toolbar_len, GetWindowHeight()))
		return 2; //矩形
	else if (in_box(x, y, GetWindowWidth() / 2, GetWindowWidth() / 2 + toolbar_len,
					GetWindowHeight() - toolbar_len, GetWindowHeight()))
		return 3; //圆
	else if (in_box(x, y, GetWindowWidth() / 2 + toolbar_len, GetWindowWidth() / 2 + 2 * toolbar_len,
					GetWindowHeight() - toolbar_len, GetWindowHeight()))
		return 4; //文本
	else if (in_box(x, y, GetWindowWidth() / 2 + 2 * toolbar_len, GetWindowWidth() / 2 + 3 * toolbar_len,
					GetWindowHeight() - toolbar_len, GetWindowHeight()))
		return 5; //帮助
	else		  //在菜单栏之外，命令指令为0
		return 0; //无命令
}

void show_instruction(bool judge)
{ //显示操作说明
	bool a = GetEraseMode();
	SetEraseMode(!judge); //记录当前擦除模式和字体大小
	int size = GetPointSize();
	SetPointSize(instruction_size);

	MovePen(GetWindowWidth() / 2 + 3.3 * toolbar_len,
			GetWindowHeight() - 0.25);
	DrawTextString("鼠标左键选中工具栏的元素后，在屏");

	MovePen(GetWindowWidth() / 2 + 3.3 * toolbar_len,
			GetWindowHeight() - 0.75);
	DrawTextString("幕上按住左键拖动鼠标即可创建图形。");

	MovePen(GetWindowWidth() / 2 + 3.3 * toolbar_len,
			GetWindowHeight() - 1.25);
	DrawTextString("选中已有图形后，按住左键拖动鼠标");

	MovePen(GetWindowWidth() / 2 + 3.3 * toolbar_len,
			GetWindowHeight() - 1.75);
	DrawTextString("即可移动图形，按右键可改变大小。");

	MovePen(GetWindowWidth() / 2 + 3.3 * toolbar_len,
			GetWindowHeight() - 2.25);
	DrawTextString("再次单击屏幕上的帮助栏，可以让提");

	MovePen(GetWindowWidth() / 2 + 3.3 * toolbar_len,
			GetWindowHeight() - 2.75);
	DrawTextString("示消失。更详细的说明可参见文件夹");

	MovePen(GetWindowWidth() / 2 + 3.3 * toolbar_len,
			GetWindowHeight() - 3.25);
	DrawTextString("中的\"使用说明\"文本文档。");

	SetPointSize(size); //恢复之前的擦除模式和字体大小
	SetEraseMode(a);
}


