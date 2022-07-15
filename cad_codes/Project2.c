/*
编写一个小型 CAD 程序，要求：
1) 支持直线、矩形、椭圆和文字四种图形元素；
2) 实现任意放置以及选中后删除、选中后改变大小、选中后移动三种功能；
3) 每种图形元素都支持绘制任意个数；
4) 利用四种图形元素的组合任意绘制一种图案；
5) 各种图形元素须分别定义数据结构，并建立相应的图形元素处理函数库；
6) 须采用抽象链表结构来存储各种图形元素。
*/

#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "random.h"
#include "strlib.h"
#include "conio.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stddef.h>
#include <windows.h>
#include <olectl.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>

#include "linkedlist.h"
#include "toolbar.h"

#include "line.h"
#include "rectangle.h"
#include "ellipse.h"
#include "text.h"

#define SYSCOLOR "Black"
#define SYSSIZE 1
#define default_len 1

static bool isDraw = FALSE;	//是否为画图状态
static bool isWrite = FALSE;//是否为输入文本状态

static bool isEdit = FALSE;	//是否为编辑状态
static bool isMove = FALSE;	//是否移动
static int isChangeSize = 0;//改变大小的模式，0表示不移动，1~4代表移动左下角、右下角等状态
static bool isShowInstruction = FALSE;//是否显示操纵提示
static int default_pen_size;
static int max_pen_size = 64;

static int create_order = 0; //表示菜单栏创建图形的命令
static int edit_order = 0;	 //编辑图形时用到的指令
static int edit_order_tmp;	 //记录上一次编辑的指令

static void *obj;			//编辑图形时被选中的元素
static void *obj_tmp = NULL;//记录上一次被选中的元素

static LineT linePtr = NULL; //将直线存入链表用到的中间变量
static RectT rectPtr = NULL; //将矩形存入链表用到的中间变量
static ElliT elliPtr = NULL; //将椭圆存入链表用到的中间变量
static textT curtext = NULL; //将文本存入链表用到的中间变量

static LineT objline = NULL; //选中直线时用到的中间变量
static RectT objrect = NULL; //选中矩形时用到的中间变量
static ElliT objelli = NULL; //选中椭圆时用到的中间变量
static textT objtext = NULL; //选中文本时用到的中间变量

linkedlistADT list1; //存储直线的链表
linkedlistADT list2; //存储矩形的链表
linkedlistADT list3; //存储椭圆的链表
linkedlistADT list4; //存储文本的链表

void KeyboardEventProcess(int key, int event);				 /*键盘消息回调函数*/
void CharEventProcess(char c);								 /*字符消息回调函数*/
void MouseEventProcess(int x, int y, int button, int event); /*鼠标消息回调函数*/
void TimerEventProcess(int timerID);						 /*定时器消息回调函数*/

void refresh(bool judge);	//刷新所有图形元素和操作说明
void highlight(void *obj, int edit_order, int judge);//被选中的元素突出显示

void Main() //仅初始化执行一次
{
	InitGraphics();	 //图形初始化

	draw_toolbar(0); //画菜单栏

	registerKeyboardEvent(KeyboardEventProcess); /*注册键盘消息回调函数*/
	registerCharEvent(CharEventProcess);		 /*注册字符消息回调函数*/
	registerMouseEvent(MouseEventProcess);		 /*注册鼠标消息回调函数*/
	registerTimerEvent(TimerEventProcess);		 /*注册定时器消息回调函数*/

	SetPenSize(SYSSIZE);   //粗细
	SetPenColor(SYSCOLOR); //颜色
	default_pen_size = GetPointSize();//default_pen_size = 13;

	list1 = NewLinkedList();	//初始化链表
	list2 = NewLinkedList();
	list3 = NewLinkedList();
	list4 = NewLinkedList();
}

 /*键盘消息回调函数*/
void KeyboardEventProcess(int key, int event) /*每当产生键盘消息，都要执行*/
{
 	switch (event)
 	{
 	case KEY_DOWN:
		if (isWrite)
		{	//如果正在创建文本
 			DrawTextT(curtext, HIDDEN);	 /*擦除当前文本*/
 			DrawCursor(curtext, HIDDEN); /*隐藏光标*/
 			if (key == VK_LEFT)
 			{ /*左移键*/
 				CursorMoveLeft(curtext);
 			}
 			else if (key == VK_RIGHT)
 			{ /*右移键*/
 				CursorMoveRight(curtext);
 			}
 			else if (key == VK_DELETE)
 			{								/*DELETE*/
 				DeleteCurrentChar(curtext); /*删除当前光标位置的字符*/
 			}
 			else if (key == VK_BACK)
 			{							/*Backspace*/
 				BackspaceChar(curtext); /*删除当前光标位置左边的一个字符*/
 			}
 			DrawTextT(curtext, DISPLAY);  /*显示当前文本*/
 			DrawCursor(curtext, DISPLAY); /*显示光标*/
 			break;
 		}
 		if (obj != NULL && key == VK_BACK)
 		{	//如果按了退格键且create_order=0(非创立文本状态)，选中的元素删除
 			switch (edit_order)
 			{
 			case 1:
 				SetEraseMode(TRUE);
 				TraverseLinkedList(list1, Line1);//擦除
 				DeleteNode(list1, obj, NULL);	//删除节点
 				SetEraseMode(FALSE);
 				TraverseLinkedList(list1, Line1);//复现
 				edit_order = 0;	//编辑完成，这些状态初始化
				obj = NULL;
				edit_order_tmp = 0;
				obj_tmp = NULL;
 				break;
 			case 2:
 				SetEraseMode(TRUE);
 				TraverseLinkedList(list2, Rect1);
 				DeleteNode(list2, obj, NULL);
 				SetEraseMode(FALSE);
 				TraverseLinkedList(list2, Rect1);
				edit_order = 0;
				obj = NULL;
				edit_order_tmp = 0;
				obj_tmp = NULL;
				break;
 			case 3:
 				SetEraseMode(TRUE);
 				TraverseLinkedList(list3, Elli1);
 				DeleteNode(list3, obj, NULL);
 				SetEraseMode(FALSE);
 				TraverseLinkedList(list3, Elli1);
				edit_order = 0;
				obj = NULL;
				edit_order_tmp = 0;
				obj_tmp = NULL;
				break;
 			case 4:
 				SetEraseMode(TRUE);
 				TraverseLinkedList(list4, RefreshText);
 				DeleteNode(list4, obj, NULL);
 				SetEraseMode(FALSE);
 				TraverseLinkedList(list4, RefreshText);
				edit_order = 0;
				obj = NULL;
				edit_order_tmp = 0;
				obj_tmp = NULL;
				break;
 			}
 		}
 		break;
 	case KEY_UP:
 		break;
 	}
 	return;
}

/*字符消息回调函数*/
void CharEventProcess(char c)
{
	if(isWrite)
	{	//如果正在创立文本
		switch (c)
		{
		case '\b': /*BACKSPACE已在KeyboardEventProcess()中处理*/
			break;
		case 27:   //Esc
		case '\r': //回车
			create_order = 0;//按回车或Esc表示输入结束
			isWrite = FALSE;
			curtext->inText = FALSE;			/*取消当前文本编辑状态*/
			DrawCursor(curtext, 1);				/*擦除当前光标*/
			curtext->isCursorDisplayed = FALSE; /*取消当前文本的光标显示状态*/
			cancelTimer(CURSOR_BLINK);			/*注销光标闪烁定时器*/
			if (stringlenth(curtext) == 0)		//如果没写，删除节点
				DeleteNode(list4, curtext, NULL);
			draw_toolbar(0);
			break;
		default:
			if (strlen(curtext->textbuf) >= TEXTLEN)
				break;					  /*超出文本最大值*/
			DrawTextT(curtext, HIDDEN);	  /*擦除原文本*/
			DrawCursor(curtext, HIDDEN);  /*隐藏光标*/
			InputChar(curtext, c);		  /*插入字符*/
			DrawTextT(curtext, DISPLAY);  /*显示新文本*/
			DrawCursor(curtext, DISPLAY); /*显示光标*/
			break;
		}
	}
	else if (create_order != 0 && c == 27) //Esc
	{	//如果不是输入文本状态，且即将创立元素，未创立的元素取消创立
		create_order = 0;
		draw_toolbar(0);
	}
	else if(obj != NULL && c == 27)
	{	//选中了元素，按Esc后取消选中
		highlight(obj, edit_order, 0);
		isEdit = 0;
		obj = NULL;
		edit_order = 0;
		obj_tmp = NULL;
		edit_order_tmp = 0;
	}
	
	return;
}

/*鼠标消息回调函数，其中(x,y)是鼠标在屏幕上的当前位置，单位：像素*/
void MouseEventProcess(int x, int y, int button, int event)
{
	double mx, my;				  /*当前鼠标坐标*/
	static double omx, omy;		  /*记录上一次鼠标坐标*/
	static double x1, y1, x2, y2; //用于画直线和矩形
	static double x0, y0, a, b;

	mx = ScaleXInches(x); /*pixels --> inches*/
	my = ScaleYInches(y); /*pixels --> inches*/

	switch (event)
	{
	case BUTTON_DOWN:
		if (choice(mx, my) == 5) //如果单击帮助栏，显示/关闭操作提示
		{
			isShowInstruction = !isShowInstruction;
			show_instruction(isShowInstruction);//显示/关闭操作提示 
			refresh(TRUE);	//防止文字覆盖擦除其他内容，故刷新 
			highlight(obj, edit_order, isEdit);
			draw_boundary();

			break;
		}

		if (isWrite) //如果正在输入文本，输入结束
		{
			create_order = choice(mx, my);
			isWrite = FALSE;
			curtext->inText = FALSE;			/*取消当前文本编辑状态*/
			DrawCursor(curtext, 1);				/*擦除当前光标*/
			curtext->isCursorDisplayed = FALSE; /*取消当前文本的光标显示状态*/
			cancelTimer(CURSOR_BLINK);			/*注销光标闪烁定时器*/
			if (stringlenth(curtext) == 0)		//如果没写，删除节点
				DeleteNode(list4, curtext, NULL);
			draw_toolbar(0);
		}

		if (choice(mx, my) != 0 && choice(mx, my) != 5)
		{										   //如果单击菜单栏，表示选中状态，此时不再执行后续操作
			highlight(obj_tmp, edit_order_tmp, 0); //之前选中的图形不再突出显示
			isEdit = 0;
			obj = NULL;
			edit_order = 0;
			obj_tmp = NULL;
			edit_order_tmp = 0;
			create_order = choice(mx, my);	   //create_order存储选中的指令
			draw_toolbar(create_order);			   //被选择的图案突出显示
			break;								   //不执行以下操作，进入BUTTON_UP
		}

		//下面这段代码等价于obj = TraverseChoose(); edit_order = get_order(obj);
		objline = TraverseChoose(list1, mx, my, InLine); //选中鼠标所在位置的直线
		objrect = TraverseChoose(list2, mx, my, InRec);	 //选中鼠标所在位置的矩形
		objelli = TraverseChoose(list3, mx, my, InElli); //选中鼠标所在位置的椭圆
		objtext = TraverseChoose(list4, mx, my, InText); //选中鼠标所在位置的文字
		obj = NULL;
		edit_order = 0;
		if (objelli != NULL)
		{
			obj = (void *)objelli;
			edit_order = 3;
		}
		if (objrect != NULL)
		{
			obj = (void *)objrect;
			edit_order = 2;
		}
		if (objline != NULL)
		{
			obj = (void *)objline;
			edit_order = 1;
		}
		if (objtext != NULL)
		{
			obj = (void *)objtext;
			edit_order = 4;
		}
		//以上代码等价于obj = TraverseChoose(); edit_order = get_order(obj);
		//这个选中原理导致了，如果图形重叠，优先级 文字 > 直线 > 矩形 > 椭圆
		//						同种图案的优先级 先创立的 > 后创立的

		if(obj == NULL)
		{	//如果什么都没选中，之前选中的元素取消选中
			highlight(obj_tmp, edit_order_tmp, 0);
			obj_tmp = NULL;
			edit_order_tmp = 0;
		}

		if (create_order == 0 && obj != NULL && obj_tmp != obj) //第一次选中某个图形元素，break
		{
			highlight(obj_tmp, edit_order_tmp, 0);
			highlight(obj, edit_order, 1);
			obj_tmp = obj;
			edit_order_tmp = edit_order;
			break;
		}

		switch (create_order)
		{
		case 1: //指令为1，画直线
			if (button == LEFT_BUTTON)
			{
				x1 = mx;
				y1 = y2 = my;
				x2 = x1 + default_len;	//若只单击鼠标，设置默认长度
				isDraw = TRUE;
				Line(x1, y1, x2, y2);
			}

			obj = NULL; //之前选中的图形不管它
			edit_order = 0;
			obj_tmp = NULL;
			edit_order_tmp = 0;
			break;

		case 2: //指令为2，画矩形
			if (button == LEFT_BUTTON)
			{
				x1 = mx;
				y1 = my;
				x2 = x1 + default_len;	//默认长度
				y2 = y1 - default_len;	//默认长度
				isDraw = TRUE;
				Rect(x1, y1, x2, y2);
			}

			obj = NULL; //之前选中的图形不管它
			edit_order = 0;
			obj_tmp = NULL;
			edit_order_tmp = 0;
			break;

		case 3: //指令为3，画圆
			if (button == LEFT_BUTTON)
			{
				a = b = default_len;	//默认半径
				x1 = x0 = mx;
				y1 = y0 = my;
				isDraw = TRUE;
				Elli(x0, y0, a, b);
			}

			obj = NULL; //之前选中的图形不管它
			edit_order = 0;
			obj_tmp = NULL;
			edit_order_tmp = 0;
			break;

		case 4: //指令为4，输入文本
			isWrite = TRUE;
			curtext = NewText(); /*开辟新的文本存储空间*/
			curtext->textx = mx; /*设置文本起始位置*/
			curtext->texty = my;
			curtext->textbuf[0] = '\0';						 /*形成空字符串*/
			curtext->curPos = 0;							 /*光标初始位置（字符串中第几个字节）*/
			curtext->isCursorDisplayed = TRUE;				 /*设置光标显示标志*/
			curtext->timerseconds = 500;					 /*设置光标显示的间隔时间，单位：ms*/
			curtext->inText = TRUE;							 /*设置文本处于编辑状态*/
			DrawCursor(curtext, DISPLAY);					 /*显示光标*/
			startTimer(CURSOR_BLINK, curtext->timerseconds); /*启动光标闪烁定时器*/
			InsertNode(list4, NULL, curtext);				 /*将当前文本插入链表*/

			obj = NULL; //之前选中的图形不管它
			edit_order = 0;
			obj_tmp = NULL;
			edit_order_tmp = 0;
			break;

		case 0:
			break;
		}

		switch (edit_order)
		{
		case 1: //选中的是直线
			isEdit = TRUE;
			if (button == LEFT_BUTTON)
				isMove = TRUE; //改位置
			else if (button == RIGHT_BUTTON)
			{
				if (fabs(mx - objline->x2) <= fabs(mx - objline->x1))
					isChangeSize = 1; //选出鼠标最接近的顶点
				else if (fabs(mx - objline->x2) > fabs(mx - objline->x1))
					isChangeSize = 2;
			}
			break;
		case 2: //选中的是矩形
			isEdit = TRUE;
			if (button == LEFT_BUTTON)
				isMove = TRUE; //改位置
			else if (button == RIGHT_BUTTON)
			{	//改大小
				if (fabs(mx - objrect->x2) <= fabs(mx - objrect->x1) &&
					fabs(my - objrect->y2) <= fabs(my - objrect->y1)) //移动右下角
					isChangeSize = 1;
				else if (fabs(mx - objrect->x2) <= fabs(mx - objrect->x1) &&
						 fabs(my - objrect->y2) > fabs(my - objrect->y1)) //移动右上角
					isChangeSize = 2;
				else if (fabs(mx - objrect->x2) > fabs(mx - objrect->x1) &&
						 fabs(my - objrect->y2) <= fabs(my - objrect->y1)) //移动左下角
					isChangeSize = 3;
				else if (fabs(mx - objrect->x2) > fabs(mx - objrect->x1) &&
						 fabs(my - objrect->y2) > fabs(my - objrect->y1)) //移动左上角
					isChangeSize = 4;
			}
			break;
		case 3: //选中的是圆
			isEdit = TRUE;
			if (button == LEFT_BUTTON)
				isMove = TRUE; //改位置
			else if (button == RIGHT_BUTTON)
				isChangeSize = 1; //改大小
			break;
		case 4: //选中的是文本
			isEdit = TRUE;
			if (button == LEFT_BUTTON)
				isMove = TRUE;//改位置
			else if (button == RIGHT_BUTTON)
				isChangeSize = 1; //改大小
			break;
		case 0:
			break;
		}

		omx = mx;
		omy = my;

		break;

	case BUTTON_UP:
		if (choice(mx, my) != 0) //如果鼠标停留在菜单栏，create_order!=0，但希望什么都不做
			break;
		
		switch (create_order)
		{
		case 1:
			if (button == LEFT_BUTTON)
			{ //左键弹起，将刚画好的图形存入链表
				isDraw = FALSE;
				linePtr = New(LineT);
				linePtr->x1 = x1;
				linePtr->y1 = y1;
				linePtr->x2 = x2;
				linePtr->y2 = y2;
				InsertNode(list1, NULL, linePtr);
				create_order = 0; //create_order归零
				draw_toolbar(0);
			}
			break;
		case 2:
			if (button == LEFT_BUTTON)
			{
				isDraw = FALSE;
				rectPtr = New(RectT);
				rectPtr->x1 = x1;
				rectPtr->y1 = y1;
				rectPtr->x2 = x2;
				rectPtr->y2 = y2;
				InsertNode(list2, NULL, rectPtr);
				create_order = 0;
				draw_toolbar(0);
			}
			break;
		case 3:
			if (button == LEFT_BUTTON)
			{
				isDraw = FALSE;
				elliPtr = New(ElliT);
				elliPtr->x = x0;
				elliPtr->y = y0;
				elliPtr->a = a;
				elliPtr->b = b;
				InsertNode(list3, NULL, elliPtr);
				create_order = 0;
				draw_toolbar(0);
			}
			break;
		case 4:
			break;
		}

		switch (edit_order)
		{
		case 1: //选中的是直线
		case 2: //选中的是矩形
		case 3: //选中的是圆
			if (button == LEFT_BUTTON)
				isMove = FALSE; //改位置
			else if (button == RIGHT_BUTTON)
				isChangeSize = FALSE; //改大小
			break;
		case 4: //选中的是文本
			if (button == LEFT_BUTTON)
				isMove = FALSE;//改位置
			else if (button == RIGHT_BUTTON)
			{
				isChangeSize = FALSE; //改大小
				SetPointSize(default_pen_size);
			}
			break;
		}

		break;

	case MOUSEMOVE:
		if (isDraw)
		{
			switch (create_order)
			{
			case 1: //如果order不为0，一定是正在画某个图形(画完后order会归0)
				SetEraseMode(TRUE);
				refresh(FALSE);	//删除前面的图形
				Line(x1, y1, x2, y2);//画新图形
				x2 = mx;
				y2 = my;
				SetEraseMode(FALSE);
				refresh(TRUE);	//前面的图形复现
				Line(x1, y1, x2, y2);
				draw_toolbar(create_order);
				break;
			case 2:
				SetEraseMode(TRUE);
				refresh(FALSE);
				Rect(x1, y1, x2, y2);
				x2 = mx;
				y2 = my;
				SetEraseMode(FALSE);
				refresh(TRUE);
				Rect(x1, y1, x2, y2);
				draw_toolbar(create_order);
				break;
			case 3:
				SetEraseMode(TRUE);
				refresh(FALSE);
				Elli(x0, y0, a, b);
				x0 = (mx + x1) / 2;
				y0 = (my + y1) / 2;
				a = fabs(mx - x0);
				b = fabs(my - y0);
				SetEraseMode(FALSE);
				refresh(TRUE);
				Elli(x0, y0, a, b);
				draw_toolbar(create_order);
				break;
			case 4:
				break;
			}
		}

		if (isEdit)
		{
			switch (edit_order)
			{
			case 1:
				if (isMove)
				{ //左键改位置
					refresh(FALSE);
					objline->x1 += (mx - omx);
					objline->x2 += (mx - omx);
					objline->y1 += (my - omy);
					objline->y2 += (my - omy);
					omx = mx;
					omy = my;
					refresh(TRUE);
					highlight(obj, edit_order, 1);
					draw_toolbar(create_order);
				}
				else if (isChangeSize)
				{ //右键改大小
					refresh(FALSE);
					switch (isChangeSize)
					{
					case 1:	//改右端点
						objline->x2 += (mx - omx);
						objline->y2 += (my - omy);
						break;
					case 2:	//改左端点
						objline->x1 += (mx - omx);
						objline->y1 += (my - omy);
					}
					omx = mx;
					omy = my;
					refresh(TRUE);
					highlight(obj, edit_order, 1);
					draw_toolbar(create_order);
				}
				break;

			case 2:
				if (isMove)
				{ //左键改位置
					refresh(FALSE);
					objrect->x1 += (mx - omx);
					objrect->x2 += (mx - omx);
					objrect->y1 += (my - omy);
					objrect->y2 += (my - omy);
					omx = mx;
					omy = my;
					refresh(TRUE);
					highlight(obj, edit_order, 1);
					draw_toolbar(create_order);
				}
				else if (isChangeSize)
				{ //右键改大小
					refresh(FALSE);
					switch (isChangeSize)
					{
					case 1://右下角
						objrect->x2 += (mx - omx);
						objrect->y2 += (my - omy);
						break;
					case 2://右上角
						objrect->x2 += (mx - omx);
						objrect->y1 += (my - omy);
						break;
					case 3://左下角
						objrect->x1 += (mx - omx);
						objrect->y2 += (my - omy);
						break;
					case 4://左上角
						objrect->x1 += (mx - omx);
						objrect->y1 += (my - omy);
						break;
					}
					omx = mx;
					omy = my;
					refresh(TRUE);
					highlight(obj, edit_order, 1);
					draw_toolbar(create_order);
				}

				break;

			case 3:
				if (isMove)
				{ //左键改位置
					refresh(FALSE);
					objelli->x += (mx - omx);
					objelli->y += (my - omy);
					omx = mx;
					omy = my;
					refresh(TRUE);
					highlight(obj, edit_order, 1);
					draw_toolbar(create_order);
				}
				else if (isChangeSize)
				{ //右键改大小
					refresh(FALSE);
					if (mx > objelli->x)
						objelli->a += (mx - omx);
					else
						objelli->a += (omx - mx);
					if (my > objelli->y)
						objelli->b += (my - omy);
					else
						objelli->b += (omy - my);
					omx = mx;
					omy = my;
					refresh(TRUE);
					highlight(obj, edit_order, 1);
					draw_toolbar(create_order);
				}
				break;
			case 4:
				if (isMove)
				{ //左键改位置
					refresh(FALSE);
					objtext->textx += (mx - omx);
					objtext->texty += (my - omy);
					omx = mx;
					omy = my;
					refresh(TRUE);
					highlight(obj, edit_order, 1);
					draw_toolbar(create_order);
				}
				else if (isChangeSize)
				{ //右键改大小
					refresh(FALSE);
					if (objtext->pen_size >= default_pen_size && objtext->pen_size <= max_pen_size)
						objtext->pen_size += ((my - omy) * 80);
					else if (objtext->pen_size < default_pen_size)
						objtext->pen_size = default_pen_size;
					else if (objtext->pen_size > max_pen_size)
						objtext->pen_size = max_pen_size;
						
					omx = mx;
					omy = my;
					refresh(TRUE);
					highlight(obj, edit_order, 1);
					draw_toolbar(create_order);
				}
				break;
			case 0:
				break;
			}
		}
	}
	return;
}

/*定时器消息回调函数*/
void TimerEventProcess(int timerID)
{
	switch (timerID)
	{
	case CURSOR_BLINK:											  /*500ms光标闪烁定时器*/
		curtext->isCursorDisplayed = !curtext->isCursorDisplayed; /*翻转当前光标显示/隐藏状态*/
		DrawCursor(curtext, curtext->isCursorDisplayed);		  /*显示/隐藏光标*/
		DrawTextT(curtext, DISPLAY);							  /*刷新文本字符串*/
		break;
	}
	return;
}

//编辑图形时擦除其他图形，故刷新
void refresh(bool judge)
{
	bool a = GetEraseMode();

	SetEraseMode(!judge);
	TraverseLinkedList(list1, Line1);
	TraverseLinkedList(list2, Rect1);
	TraverseLinkedList(list3, Elli1);
	TraverseLinkedList(list4, RefreshText);
	show_instruction(isShowInstruction);

	SetEraseMode(a);
}

//让被选中的图形突出显示
void highlight(void *obj, int edit_order, int judge)
{
	if (obj == NULL || edit_order == 0)
		return;

	switch (edit_order)
	{
	case 1:
		highlight_line(obj, judge);
		break;
	case 2:
		highlight_rect(obj, judge);
		break;
	case 3:
		highlight_ell(obj, judge);
		break;
	case 4:
		highlight_text(obj, judge);
		break;
	}
}


