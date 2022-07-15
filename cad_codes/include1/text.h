/*
 *text.h
 */

#ifndef _TEXT_H_
#define _TEXT_H_

#include "genlib.h"
#include "graphics.h"
#include "extgraph.h"
#include "strlib.h"

#define CURSOR 			"_"		/*光标符号*/
#define CURSOR_BLINK  	1		/*光标闪烁定时器事件标志号*/
#define TEXTLEN  		200		/*文本最大长度*/
#define HIDDEN  		TRUE	/*隐藏文本/光标的标志*/
#define DISPLAY 		FALSE	/*显示文本/光标的标志*/

struct _text { /*文本结构类型*/
	char textbuf[TEXTLEN+1];/*当前文本缓冲区*/
	double textx, texty; /*文本显示起始位置*/
	int curPos; /*光标当前位置*/
	bool inText;  /*是否处于文本编辑状态*/
	bool isCursorDisplayed;  /*光标是否显示*/ 
	int timerseconds; /*光标闪烁周期ms*/
	int pen_size;	//字体大小，初始化时为默认大小13
};
typedef struct _text *textT; /*文本指针类型*/

textT NewText(); /*新建一个空的文本指针*/
void DrawTextT(textT text, bool mode); /*显示/擦除文本*/
void DrawCursor(textT text, bool mode);/*显示/隐藏光标*/
void CursorMoveLeft(textT text); /*光标左移*/
void CursorMoveRight(textT text); /*光标右移*/
void DeleteCurrentChar(textT text);/*删除当前光标位置的字符*/
void BackspaceChar(textT text); /*删除光标左边字符*/
void InputChar(textT text, char c);/*输入字符*/

void InsertCharToString(string str, int pos, char c);/*字符串str的pos位置插入字符c*/
void DeleteCharFromString(string str, int pos);/*删除字符串str在pos位置的字符*/
bool isHzByte(char c); /*判断是否汉字字节*/
int stringlenth(textT text);

bool InText(double mx, double my, void *obj);
void RefreshText(void *obj);
void highlight_text(void *obj, int judge);

#endif /*_TEXT_H_*/

