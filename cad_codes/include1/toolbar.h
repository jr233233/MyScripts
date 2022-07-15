#include "genlib.h"
#include "graphics.h"
#include "extgraph.h"
#include "strlib.h"

#define toolbar_len 0.7	   //�������Ŀ��
#define instruction_size 13 //˵�����ֵĴ�С

void draw_box(double len, double wid)
{	//������
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
{	//���߽���
	MovePen(0.1, 0.1);
	draw_box(GetWindowWidth() - 0.2, GetWindowHeight() - toolbar_len - 0.1);
}

void draw_toolbar(int order)
{ //��������������order��Ӧ����ͻ����ʾ(���)
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
	}//������5������

	MovePen(GetWindowWidth() / 2 - 9 * toolbar_len,
			GetWindowHeight() - 0.55 * toolbar_len);
	DrawTextString("���ڱ߿������ڻ�ͼ");	//д��ʾ���

	MovePen(GetWindowWidth() / 2 - 4 * toolbar_len,
			GetWindowHeight() - 0.55 * toolbar_len);
	DrawTextString("������");

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
	SetEraseMode(FALSE); /*ȡ������ģʽ*/
	DrawTextString("�ı�");

	MovePen(GetWindowWidth() / 2 + 2.15 * toolbar_len,
			GetWindowHeight() - 0.6 * toolbar_len);
	SetEraseMode(FALSE); /*ȡ������ģʽ*/
	DrawTextString("����");

	draw_boundary();

	MovePen(GetWindowWidth() / 2, GetWindowHeight());

	if (order == 0)
		return;
	else
	{ //ͻ����ʾָ������1��Ӧֱ�ߣ�2��Ӧ���Σ�3��Ӧ��Բ��4��Ӧ�ı�
		MovePen(GetWindowWidth() / 2 + (order - 3) * toolbar_len, GetWindowHeight());
		SetPenColor("Red");
		DrawLine(toolbar_len, 0);
		DrawLine(0, -toolbar_len);
		DrawLine(-toolbar_len, 0);
		DrawLine(0, toolbar_len);
		SetPenColor("Black");
	}
}

int choice(double x, double y) //�ж�(x,y)λ����һ��������
{
	if (in_box(x, y, GetWindowWidth() / 2 - 2 * toolbar_len, GetWindowWidth() / 2 - toolbar_len,
			   GetWindowHeight() - toolbar_len, GetWindowHeight()))
		return 1; //ֱ��
	else if (in_box(x, y, GetWindowWidth() / 2 - toolbar_len, GetWindowWidth() / 2,
					GetWindowHeight() - toolbar_len, GetWindowHeight()))
		return 2; //����
	else if (in_box(x, y, GetWindowWidth() / 2, GetWindowWidth() / 2 + toolbar_len,
					GetWindowHeight() - toolbar_len, GetWindowHeight()))
		return 3; //Բ
	else if (in_box(x, y, GetWindowWidth() / 2 + toolbar_len, GetWindowWidth() / 2 + 2 * toolbar_len,
					GetWindowHeight() - toolbar_len, GetWindowHeight()))
		return 4; //�ı�
	else if (in_box(x, y, GetWindowWidth() / 2 + 2 * toolbar_len, GetWindowWidth() / 2 + 3 * toolbar_len,
					GetWindowHeight() - toolbar_len, GetWindowHeight()))
		return 5; //����
	else		  //�ڲ˵���֮�⣬����ָ��Ϊ0
		return 0; //������
}

void show_instruction(bool judge)
{ //��ʾ����˵��
	bool a = GetEraseMode();
	SetEraseMode(!judge); //��¼��ǰ����ģʽ�������С
	int size = GetPointSize();
	SetPointSize(instruction_size);

	MovePen(GetWindowWidth() / 2 + 3.3 * toolbar_len,
			GetWindowHeight() - 0.25);
	DrawTextString("������ѡ�й�������Ԫ�غ�����");

	MovePen(GetWindowWidth() / 2 + 3.3 * toolbar_len,
			GetWindowHeight() - 0.75);
	DrawTextString("Ļ�ϰ�ס����϶���꼴�ɴ���ͼ�Ρ�");

	MovePen(GetWindowWidth() / 2 + 3.3 * toolbar_len,
			GetWindowHeight() - 1.25);
	DrawTextString("ѡ������ͼ�κ󣬰�ס����϶����");

	MovePen(GetWindowWidth() / 2 + 3.3 * toolbar_len,
			GetWindowHeight() - 1.75);
	DrawTextString("�����ƶ�ͼ�Σ����Ҽ��ɸı��С��");

	MovePen(GetWindowWidth() / 2 + 3.3 * toolbar_len,
			GetWindowHeight() - 2.25);
	DrawTextString("�ٴε�����Ļ�ϵİ���������������");

	MovePen(GetWindowWidth() / 2 + 3.3 * toolbar_len,
			GetWindowHeight() - 2.75);
	DrawTextString("ʾ��ʧ������ϸ��˵���ɲμ��ļ���");

	MovePen(GetWindowWidth() / 2 + 3.3 * toolbar_len,
			GetWindowHeight() - 3.25);
	DrawTextString("�е�\"ʹ��˵��\"�ı��ĵ���");

	SetPointSize(size); //�ָ�֮ǰ�Ĳ���ģʽ�������С
	SetEraseMode(a);
}


