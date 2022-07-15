/*
 *text.h
 */

#ifndef _TEXT_H_
#define _TEXT_H_

#include "genlib.h"
#include "graphics.h"
#include "extgraph.h"
#include "strlib.h"

#define CURSOR 			"_"		/*������*/
#define CURSOR_BLINK  	1		/*�����˸��ʱ���¼���־��*/
#define TEXTLEN  		200		/*�ı���󳤶�*/
#define HIDDEN  		TRUE	/*�����ı�/���ı�־*/
#define DISPLAY 		FALSE	/*��ʾ�ı�/���ı�־*/

struct _text { /*�ı��ṹ����*/
	char textbuf[TEXTLEN+1];/*��ǰ�ı�������*/
	double textx, texty; /*�ı���ʾ��ʼλ��*/
	int curPos; /*��굱ǰλ��*/
	bool inText;  /*�Ƿ����ı��༭״̬*/
	bool isCursorDisplayed;  /*����Ƿ���ʾ*/ 
	int timerseconds; /*�����˸����ms*/
	int pen_size;	//�����С����ʼ��ʱΪĬ�ϴ�С13
};
typedef struct _text *textT; /*�ı�ָ������*/

textT NewText(); /*�½�һ���յ��ı�ָ��*/
void DrawTextT(textT text, bool mode); /*��ʾ/�����ı�*/
void DrawCursor(textT text, bool mode);/*��ʾ/���ع��*/
void CursorMoveLeft(textT text); /*�������*/
void CursorMoveRight(textT text); /*�������*/
void DeleteCurrentChar(textT text);/*ɾ����ǰ���λ�õ��ַ�*/
void BackspaceChar(textT text); /*ɾ���������ַ�*/
void InputChar(textT text, char c);/*�����ַ�*/

void InsertCharToString(string str, int pos, char c);/*�ַ���str��posλ�ò����ַ�c*/
void DeleteCharFromString(string str, int pos);/*ɾ���ַ���str��posλ�õ��ַ�*/
bool isHzByte(char c); /*�ж��Ƿ����ֽ�*/
int stringlenth(textT text);

bool InText(double mx, double my, void *obj);
void RefreshText(void *obj);
void highlight_text(void *obj, int judge);

#endif /*_TEXT_H_*/

