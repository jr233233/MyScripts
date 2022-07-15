#include "text.h"
#include <string.h>

/*Insert char c to the string str at the position pos*/
void InsertCharToString(string str, int pos, char c)
{
	int len;
	
	len = strlen(str);
	if (pos < 0 || pos > len) return;
	*(str+len+1) = '\0';/*������γ��ַ���*/
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
	n = isHzByte(*(str+pos)) ? 2 : 1; /*���֣���һ��ɾ������2���ֽڣ�����ɾ��һ����ǰ�ֽ�*/
	while (*(str+pos) != '\0') {
		*(str+pos) = *(str+pos+n);
		pos++; 
	}
	
	return;
}

/*�ж��Ƿ����ֽڡ���ÿ�����ְ���2���ֽڣ���ÿ���ֽڵ����λ����1�������ֽ��Ǹ�������*/
bool isHzByte(char c)
{
	return (c < 0); 
}

/*����һ���µ��ı����󣬷��ظ��ı���ָ��*/
textT NewText()
{
	textT a;
	a = New(textT);
	a->pen_size = GetPointSize();
	return a;
}

/*��ʾ/�����ı�*/
void DrawTextT(textT text, bool mode)
{
	SetEraseMode(mode); /*����/ȡ������ģʽ*/
	MovePen(text->textx, text->texty);/*���û������*/
	DrawTextString(text->textbuf);/*��ʾ�ı�*/
	return;
}

/*��ʾ/���ع��*/
void DrawCursor(textT text, bool mode)
{
	if (text->curPos < 0 || text->curPos > strlen(text->textbuf)) return;
	SetEraseMode(mode);
	MovePen(text->textx+TextStringWidth(SubString(text->textbuf, 0, text->curPos-1)), 
	        text->texty); /*�Ƶ����λ��*/
	DrawTextString(CURSOR);/*��ʾ���*/
	return;
}

/*������ƣ������ǰһ���ֽ��Ǻ����ֽڣ����ƶ�2���ֽڣ������ƶ�1���ֽ�*/
void CursorMoveLeft(textT text)
{
	if (text->curPos <= 0) return;
    text->curPos = isHzByte(text->textbuf[text->curPos-1]) ? text->curPos-2 : text->curPos-1;
	return;
}

/*�������*/
void CursorMoveRight(textT text)
{
	if (text->curPos >= strlen(text->textbuf)) return;
    /*��ǰ�ֽ����Ǻ����ֽ���������2���ֽڣ���������1���ֽ�*/
    text->curPos = isHzByte(text->textbuf[text->curPos]) ? text->curPos+2 : text->curPos+1;
	return;
}

/*ɾ�����λ���ַ�*/ 
void DeleteCurrentChar(textT text)
{
	if (text->curPos < 0 || text->curPos >= strlen(text->textbuf)) return;
	DeleteCharFromString(text->textbuf, text->curPos);
	return;
}

/*���������ߵ��ַ�*/
void BackspaceChar(textT text)
{
	if (text->curPos <= 0) return;
	text->curPos = isHzByte(text->textbuf[text->curPos-1]) ? 
	               text->curPos-2 : text->curPos-1;/*���ǰһ���ֽ��Ǻ����ֽ�������2��*/
	DeleteCharFromString(text->textbuf, text->curPos);
	return;
}

/*�ڹ��λ�������ַ�*/
void InputChar(textT text, char c)
{
	if (strlen(text->textbuf) >= TEXTLEN) return;
	InsertCharToString(text->textbuf, text->curPos, c);/*�����ַ�*/
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
	//�߶� Ĭ�������С(13)ԼΪ0.25�߶ȣ�ͨ����γ��ԣ��ճ��˳���13���ϵ������֤���պ������ָ߶���ѡ��
	a = (my >= text->texty) && (my <= text->texty + 0.25 *(1+ (text->pen_size - 13) / 13) );
	
	//����
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
	
	MovePen(text->textx, text->texty);/*���û������*/
	DrawTextString(text->textbuf);/*��ʾ�ı�*/

	SetPointSize(a);
	return;
}

void highlight_text(void *obj, int judge)
{	//���ı�ͻ����ʾ
	if (judge == 1)
		SetPenColor("Red");
	else
		SetPenColor("Black");

	RefreshText(obj);
	SetPenColor("Black");
}



