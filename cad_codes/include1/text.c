#include "text.h"
#include <string.h>

/*Insert char c to the string str at the position pos*/
void InsertCharToString(string str, int pos, char c)
{
	int len;
	
	len = strlen(str);
	if (pos < 0 || pos > len) return;
	*(str+len+1) = '\0';/*插入后形成字符串*/
	while (len > pos) {
		*(str+len) = *(str+len-1);
		len--;
	}	
	*(str+len) = c;
	return;
}

/*Delete the character at the pos position of string str*/
void DeleteCharFromString(string str, int pos)
{
	int len, n;
	
	if (pos < 0 || pos >= (len=strlen(str))) return;
	n = isHzByte(*(str+pos)) ? 2 : 1; /*汉字，则一次删除连续2个字节，否则删除一个当前字节*/
	while (*(str+pos) != '\0') {
		*(str+pos) = *(str+pos+n);
		pos++; 
	}
	
	return;
}

/*判断是否汉字字节——每个汉字包含2个字节，且每个字节的最高位都是1（即该字节是负整数）*/
bool isHzByte(char c)
{
	return (c < 0); 
}

/*创建一个新的文本对象，返回该文本的指针*/
textT NewText()
{
	textT a;
	a = New(textT);
	a->pen_size = GetPointSize();
	return a;
}

/*显示/擦除文本*/
void DrawTextT(textT text, bool mode)
{
	SetEraseMode(mode); /*设置/取消擦除模式*/
	MovePen(text->textx, text->texty);/*设置画笔起点*/
	DrawTextString(text->textbuf);/*显示文本*/
	return;
}

/*显示/隐藏光标*/
void DrawCursor(textT text, bool mode)
{
	if (text->curPos < 0 || text->curPos > strlen(text->textbuf)) return;
	SetEraseMode(mode);
	MovePen(text->textx+TextStringWidth(SubString(text->textbuf, 0, text->curPos-1)), 
	        text->texty); /*移到光标位置*/
	DrawTextString(CURSOR);/*显示光标*/
	return;
}

/*光标左移：若光标前一个字节是汉字字节，则移动2个字节；否则移动1个字节*/
void CursorMoveLeft(textT text)
{
	if (text->curPos <= 0) return;
    text->curPos = isHzByte(text->textbuf[text->curPos-1]) ? text->curPos-2 : text->curPos-1;
	return;
}

/*光标右移*/
void CursorMoveRight(textT text)
{
	if (text->curPos >= strlen(text->textbuf)) return;
    /*当前字节若是汉字字节则光标右移2个字节，否则右移1个字节*/
    text->curPos = isHzByte(text->textbuf[text->curPos]) ? text->curPos+2 : text->curPos+1;
	return;
}

/*删除光标位置字符*/ 
void DeleteCurrentChar(textT text)
{
	if (text->curPos < 0 || text->curPos >= strlen(text->textbuf)) return;
	DeleteCharFromString(text->textbuf, text->curPos);
	return;
}

/*擦除光标左边的字符*/
void BackspaceChar(textT text)
{
	if (text->curPos <= 0) return;
	text->curPos = isHzByte(text->textbuf[text->curPos-1]) ? 
	               text->curPos-2 : text->curPos-1;/*光标前一个字节是汉字字节则左移2个*/
	DeleteCharFromString(text->textbuf, text->curPos);
	return;
}

/*在光标位置输入字符*/
void InputChar(textT text, char c)
{
	if (strlen(text->textbuf) >= TEXTLEN) return;
	InsertCharToString(text->textbuf, text->curPos, c);/*插入字符*/
	text->curPos++;
	return;
}

int stringlenth(textT text)
{
	return strlen(text->textbuf);
}

bool InText(double mx, double my, void *obj)
{
	textT text = (textT)obj;
	bool a, b;
	//高度 默认字体大小(13)约为0.25高度，通过多次尝试，凑出了除以13这个系数，保证鼠标刚好在文字高度内选中
	a = (my >= text->texty) && (my <= text->texty + 0.25 *(1+ (text->pen_size - 13) / 13) );
	
	//长度
	b = mx >= text->textx && mx <= text->textx + stringlenth(text) * 0.125 * (1 + (text->pen_size - 13) / 15);

	return a&&b;
}

void RefreshText(void *obj)
{
	textT text = (textT)obj;
	int a = GetPointSize();
	if (text->pen_size < 13)
		text->pen_size = 13;
	else if (text->pen_size > 64)
		text->pen_size = 64;
	SetPointSize(text->pen_size);
	
	MovePen(text->textx, text->texty);/*设置画笔起点*/
	DrawTextString(text->textbuf);/*显示文本*/

	SetPointSize(a);
	return;
}

void highlight_text(void *obj, int judge)
{	//让文本突出显示
	if (judge == 1)
		SetPenColor("Red");
	else
		SetPenColor("Black");

	RefreshText(obj);
	SetPenColor("Black");
}



