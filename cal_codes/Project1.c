#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "genlib.h"
#include "simpio.h"
#include "strlib.h"

#include "stack.h"

#define max_size 100
#define pi	3.1415926535897932384626433832795028841971693993751058209749445923078164
#define e	2.7182818284590452353602874713526624977572470936999595749669676277240766

bool bracket_pair(char *str);	//判断括号是否配对
double *cal(char *str);			//计算表达式 
char *cal_fun(char *str);		//把函数值全部算出，并处理一些中缀转后缀的格式要求 
char *in_to_suf(char *str);		//中缀转后缀 
double cal_suf(char *line);		//计算后缀表达式 

int main()
{
	printf("这个程序可以计算一个算式的值，目前支持sin、cos、tan、pow、exp、sqrt、log、ln函数。\n");
	printf("函数用法：sin(double theta), cos(double theta), tan(double theta)\n");
	printf("pow(double a,double b) = a^b, exp(double a), sqrt(double a), log(double a, double b), ln(double a)\n\n");
	printf("输入上述函数时，请在函数名后加括号()，然后在括号内输入参数。\n");
	printf("如果想退出程序，请输入\"quit\"。\n\n");

	char *str;
	double *result;
	while (1)
	{
		printf(": ");
		str = GetLine();
		if (StringEqual(str, "quit"))
			break;		//退出 
		strcat(str, "#");
		if (StringEqual(str, "#"))
			continue;	//空的输入，直接进入下一轮循环 

		if (!bracket_pair(str))
		{	//如果括号不配对，直接invalid input
			printf("括号不匹配，请重新输入。\n\n");
			continue;
		}

		result = cal(str);	//计算结果 
		if (result == NULL)	//如果发现语法错误，则返回NULL 
			printf("\n");	//分函数中已经提示错误，此处不再重复提示 
		else
			printf("The answer is %.2lf\n\n", *result);
	}

	return 0;
}

bool bracket_pair(char *str)	//判断括号是否配对
{
	stackADT stack;
	char *item, ch;
	int i, len;
	char *line;
	
	stack = NewStack(); /*初始化堆栈*/
	line = (char *)calloc(max_size, sizeof(char));
	strcpy(line, str);	//防止改变原字符串，申请一个新的字符串 
	
	len = StringLength(line);
	for (i = 0; i < len; i++)
	{ /*遍历输入字符串的每一个字符*/
		if (line[i] == '(' || line[i] == '[' || line[i] == '{')
		{							/*当前字符为左括号*/
			item = New(char *);		/*分配当前字符存储空间*/
			*item = line[i];		/*将该字符保存到申请到的存储空间里*/
			PushStack(stack, item); /*将该字符存储空间的指针压栈*/
		}
		else if (line[i] == ')' || line[i] == ']' || line[i] == '}')
		{																	  /*当前字符为右括号*/
			ch = *(char *)TopStack(stack);									  /*获取栈顶元素（指针）所指向的字符*/
			if (line[i] == ')' && ch == '(' || line[i] == ']' && ch == '[' || /*若是匹配的左括号*/
				line[i] == '}' && ch == '{')
				FreeBlock(PopStack(stack)); /*则出栈并释放空间*/
		}
	}
	if (IsemptyStack(stack))
	{ /*若堆栈为空，则表明所以得左右括号匹配*/
		FreeStack(stack);
		return 1;
	}
	else
	{ /*若堆栈非空，则有左右括号不匹配*/
		FreeStack(stack);
		return 0;
	}
}

double *cal(char *str)
{	//选用double*而不选double的原因：出现语法错误时，以NULL作为标记，double类型无法标记 
	char *line;
	double *res;
	line = in_to_suf(str); //中缀转后缀
	if (line == NULL)	//如果有非法输入，返回NULL 
		return NULL;

	res = (double *)calloc(1, sizeof(double));
	*res = cal_suf(line); //计算后缀表达式
	return res;
}

char *cal_fun(char *str)
{
	char *p, *str1, *tmp_p;
	char *tmp, *tmp1, *tmp2, *tmp0;
	double tmp_res1, tmp_res2, tmp_res;
	int i, j, cnt, cnt1, flag;
	p = str;
	str1 = (char *)calloc(max_size, sizeof(char));

	str1[0] = ' ';
	i = 1, j = 0;
	while (*p != '#')
	{
		switch (*p) //遍历将其中的函数值算出来，和其他部分一起，存储在字符串str1中
		{
		case 's':	//判断sin或sqrt 
			p++;
			switch (*p)
			{
			case 'i': //检测sin
				p++;
				if (*p != 'n')
				{
					printf("函数名无法识别，请重新输入。\n");
					return NULL;
				}
				p++;
				if (*p != '(')
				{	//要求sin后加括号 
					printf("sin函数使用格式不对，请重新输入。\n");
					return NULL;
				}
				//以上判断输入的是不是sin()
				//以下代码提取sin后括号内的表达式
				tmp0 = (char *)calloc(max_size, sizeof(char));
				tmp1 = (char *)calloc(max_size, sizeof(char));
				tmp = (char *)calloc(max_size, sizeof(char));

				tmp0[0] = '(';	//把括号也提取出来 
				j = 1;
				cnt = 1; //cnt记录内部左右括号的配对
				p++;
				while (1)
				{
					while (*p == ' ')
						p++;

					if (*p == '(')
						cnt++;	//左括号数++ 
					else if (*p == ')')
					{
						cnt--; //如果sin括号里面出现右括号，让它与左括号配对
						if (cnt == 0)
						{	//如果检测到右括号，且括号匹配，则提取结束 
							tmp0[j] = *p;
							j++;
							p++;
							break;
						}
					}
					tmp0[j] = *p;
					j++;
					p++;
				}
				tmp0[j] = '#';	//sin后的表达式提取至tmp0中，补上#，便于计算 
				tmp_res = sin(*cal(tmp0)); //递归，计算括号内表达式的值

				tmp[0] = '(';	 //防止sin计算结果为负数，负号与减号相互干扰
				if (tmp_res < 0) //所以不管结果正负，用括号括起来
					tmp[1] = '0';//(-tmp_res)不符合中缀转后缀规范，故写成(0-tmp_res) 
				
				tmp1 = RealToString(tmp_res);
				strcat(tmp, tmp1);
				strcat(tmp, ")");  //补上括号
				strcat(str1, tmp); //将tmp存入str1
				i += strlen(tmp);  //i用来标记str1的下标，对应增加
				free(tmp0);
				free(tmp);
				free(tmp1);
				break;

			case 'q': //检测sqrt
				p++;
				if (*p != 'r')
				{
					printf("函数名无法识别，请重新输入。\n");
					return NULL;
				}
				p++;
				if (*p != 't')
				{
					printf("函数名无法识别，请重新输入。\n");
					return NULL;
				}
				p++;
				if (*p != '(')
				{
					printf("sqrt函数使用格式不对，请重新输入。\n");
					return NULL;
				}
				//以上判断输入的是不是sqrt()
				//以下代码提取sqrt后括号内的表达式
				tmp0 = (char *)calloc(max_size, sizeof(char));
				tmp1 = (char *)calloc(max_size, sizeof(char));
				tmp = (char *)calloc(max_size, sizeof(char));

				tmp0[0] = '(';	//代码结构同sin 
				j = 1;
				cnt = 1; //cnt记录内部左右括号的配对
				p++;
				while (1)
				{
					while (*p == ' ')
						p++;

					if (*p == '(')
						cnt++;
					else if (*p == ')')
					{
						cnt--; //如果sqrt括号里面出现右括号，让它与左括号配对
						if (cnt == 0)
						{
							tmp0[j] = *p;
							j++;
							p++;
							break;
						}
					}
					tmp0[j] = *p;
					j++;
					p++;
				}
				tmp0[j] = '#';
				if (*cal(tmp0) < 0)
				{
					printf("根号(sqrt)内的数必须不小于0，请重新输入。\n");
					return NULL;
				}
				tmp_res = sqrt(*cal(tmp0)); //递归，计算括号内表达式的值
				tmp[0] = '(';				//防止函数计算结果为负数，负号与减号相互干扰
				if (tmp_res < 0)			//所以不管结果正负，用括号括起来
					tmp[1] = '0';

				sprintf(tmp1, "%.6lf", tmp_res);//若tmp_res过大，用RealToString会出现科学计数法
				strcat(tmp, tmp1); 				//不合表达式的规范，故采用sprintf 
				strcat(tmp, ")");  //补上括号
				strcat(str1, tmp); //将tmp存入str1
				i += strlen(tmp);  //i用来标记str1的下标，对应增加
				free(tmp0);
				free(tmp1);
				free(tmp);
				break;

			default:
				printf("函数名无法识别，请重新输入。\n");
				return NULL;
			}
			break;

		case 'c':	//检测cos 
			p++;
			if (*p != 'o')
			{
				printf("函数名无法识别，请重新输入。\n");
				return NULL;
			}
			p++;
			if (*p != 's')
			{
				printf("函数名无法识别，请重新输入。\n");
				return NULL;
			}
			p++;
			if (*p != '(')
			{
				printf("cos函数使用格式不对，请重新输入。\n");
				return NULL;
			}
			//以上判断输入的是不是cos()
			//以下代码提取cos后括号内的表达式
			tmp0 = (char *)calloc(max_size, sizeof(char));
			tmp1 = (char *)calloc(max_size, sizeof(char));
			tmp = (char *)calloc(max_size, sizeof(char));

			tmp0[0] = '(';
			j = 1;
			cnt = 1; //cnt记录内部左右括号的配对
			p++;
			while (1)
			{
				while (*p == ' ')
					p++;

				if (*p == '(')
					cnt++;
				else if (*p == ')')
				{
					cnt--; //如果cos括号里面出现右括号，让它与左括号配对
					if (cnt == 0)
					{
						tmp0[j] = *p;
						j++;
						p++;
						break;
					}
				}
				tmp0[j] = *p;
				j++;
				p++;
			}
			tmp0[j] = '#';
			tmp_res = cos(*cal(tmp0)); //递归，计算括号内表达式的值
			tmp[0] = '(';
			if (tmp_res < 0)
				tmp[1] = '0';
			tmp1 = RealToString(tmp_res);
			strcat(tmp, tmp1);
			strcat(tmp, ")");
			strcat(str1, tmp);
			i += strlen(tmp);
			free(tmp0);
			free(tmp);
			free(tmp1);
			break;

		case 't':	//检测tan 
			p++;
			if (*p != 'a')
			{
				printf("函数名无法识别，请重新输入。\n");
				return NULL;
			}
			p++;
			if (*p != 'n')
			{
				printf("函数名无法识别，请重新输入。\n");
				return NULL;
			}
			p++;
			if (*p != '(')
			{
				printf("tan函数使用格式不对，请重新输入。\n");
				return NULL;
			}
			//以上判断输入的是不是tan()
			//以下代码提取tan后括号内的表达式
			tmp0 = (char *)calloc(max_size, sizeof(char));
			tmp1 = (char *)calloc(max_size, sizeof(char));
			tmp = (char *)calloc(max_size, sizeof(char));

			tmp0[0] = '(';
			j = 1;
			cnt = 1; //cnt记录内部左右括号的配对
			p++;
			while (1)
			{
				while (*p == ' ')
					p++;

				if (*p == '(')
					cnt++;
				else if (*p == ')')
				{
					cnt--; //如果tan括号里面出现右括号，让它与左括号配对
					if (cnt == 0)
					{
						tmp0[j] = *p;
						j++;
						p++;
						break;
					}
				}
				tmp0[j] = *p;
				j++;
				p++;
			}
			tmp0[j] = '#';
			tmp_res = tan(*cal(tmp0)); //递归，计算括号内表达式的值

			tmp[0] = '(';
			if (tmp_res < 0)
				tmp[1] = '0';
			sprintf(tmp1, "%.6lf", tmp_res);

			strcat(tmp, tmp1);
			strcat(tmp, ")");
			strcat(str1, tmp);
			i += strlen(tmp);
			free(tmp0);
			free(tmp);
			free(tmp1);
			break;

		case 'e':	//检测exp 
			p++;
			if (*p != 'x')
			{
				printf("函数名无法识别，请重新输入。\n");
				return NULL;
			}
			p++;
			if (*p != 'p')
			{
				printf("函数名无法识别，请重新输入。\n");
				return NULL;
			}
			p++;
			if (*p != '(')
			{
				printf("exp函数使用格式不对，请重新输入。\n");
				return NULL;
			}
			//以上判断输入的是不是exp()
			//以下代码提取exp后括号内的表达式
			tmp0 = (char *)calloc(max_size, sizeof(char));
			tmp1 = (char *)calloc(max_size, sizeof(char));
			tmp = (char *)calloc(max_size, sizeof(char));

			tmp0[0] = '(';
			j = 1;
			cnt = 1; //cnt记录内部左右括号的配对
			p++;
			while (1)
			{
				while (*p == ' ')
					p++;

				if (*p == '(')
					cnt++;
				else if (*p == ')')
				{
					cnt--; //如果exp括号里面出现右括号，让它与左括号配对
					if (cnt == 0)
					{
						tmp0[j] = *p;
						j++;
						p++;
						break;
					}
				}
				tmp0[j] = *p;
				j++;
				p++;
			}
			tmp0[j] = '#';
			tmp_res = pow(e, *cal(tmp0)); //递归，计算括号内表达式的值

			tmp[0] = '(';
			if (tmp_res < 0)
				tmp[1] = '0';
			sprintf(tmp1, "%.6lf", tmp_res);

			strcat(tmp, tmp1);
			strcat(tmp, ")");
			strcat(str1, tmp);
			i += strlen(tmp);
			free(tmp0);
			free(tmp);
			free(tmp1);
			break;

		case 'p':	//检测pow 
			p++;
			if (*p != 'o')
			{
				printf("函数名无法识别，请重新输入。\n");
				return NULL;
			}
			p++;
			if (*p != 'w')
			{
				printf("函数名无法识别，请重新输入。\n");
				return NULL;
			}
			p++;
			if (*p != '(')
			{
				printf("pow函数使用格式不对，请重新输入。\n");
				return NULL;
			}
			p++;
			//以上判断输入的是不是pow()
			//以下代码提取pow后括号内的表达式，以逗号分界，分别提取
			tmp1 = (char *)calloc(max_size, sizeof(char));
			tmp2 = (char *)calloc(max_size, sizeof(char));
			j = 0;
			while (*p != ',')
			{
				while (*p == ' ')
					p++;
				tmp1[j] = *p;
				j++;
				p++;
			}
			tmp1[j] = '#';
			p++;
			j = 0;
			cnt = 1;
			while (1)
			{
				if (*p == '(')
					cnt++;
				else if (*p == ')')
				{
					cnt--;
					if (cnt == 0)
					{
						p++;
						break;
					}
				}
				tmp2[j] = *p;
				j++;
				p++;
			}
			tmp2[j] = '#';
			//递归，计算括号内表达式的值
			tmp_res1 = *cal(tmp1);
			tmp_res2 = *cal(tmp2);
			free(tmp1);
			free(tmp2);
			tmp_res = pow(tmp_res1, tmp_res2);
			tmp = (char *)calloc(max_size, sizeof(char));
			tmp0 = (char *)calloc(max_size, sizeof(char));
			tmp[0] = '(';
			if (tmp_res < 0)
				tmp[1] = '0';
			sprintf(tmp0, "%.6lf", tmp_res);
			strcat(tmp, tmp0);
			strcat(tmp, ")");
			strcat(str1, tmp);
			i += strlen(tmp);
			free(tmp0);
			free(tmp);
			break;

		case 'l':
			p++;
			switch (*p)
			{
			case 'o': //检测log
				p++;
				if (*p != 'g')
				{
					printf("函数名无法识别，请重新输入。\n");
					return NULL;
				}
				p++;
				if (*p != '(')
				{
					printf("log函数使用格式不对，请重新输入。\n");
					return NULL;
				}
				p++;
				//以上判断输入的是不是log()
				//以下代码提取log后括号内的表达式，以逗号分界，分别提取
				tmp1 = (char *)calloc(max_size, sizeof(char));
				tmp2 = (char *)calloc(max_size, sizeof(char));
				j = 0;
				while (*p != ',')
				{
					while (*p == ' ')
						p++;
					tmp1[j] = *p;
					j++;
					p++;
				}
				tmp1[j] = '#';
				p++;
				j = 0;
				cnt = 1;
				while (1)
				{
					if (*p == '(')
						cnt++;
					else if (*p == ')')
					{
						cnt--;
						if (cnt == 0)
						{
							p++;
							break;
						}
					}
					tmp2[j] = *p;
					j++;
					p++;
				}
				tmp2[j] = '#';
				//递归，计算括号内表达式的值
				tmp_res1 = *cal(tmp1);
				tmp_res2 = *cal(tmp2);
				free(tmp1);
				free(tmp2);
				if (tmp_res1 <= 0 || tmp_res1 == 1)
				{
					printf("对数的底数必须大于0且不等于1，请重新输入。\n");
					return NULL;
				}
				if(tmp_res2 <= 0)
				{
					printf("对数的真数必须大于0，请重新输入。\n");
					return NULL;
				}
				tmp_res = log(tmp_res2) / log(tmp_res1);
				tmp = (char *)calloc(max_size, sizeof(char));
				tmp0 = (char *)calloc(max_size, sizeof(char));
				tmp[0] = '(';
				if (tmp_res < 0)
					tmp[1] = '0';
				sprintf(tmp0, "%.6lf", tmp_res);
				strcat(tmp, tmp0);
				strcat(tmp, ")");
				strcat(str1, tmp);
				i += strlen(tmp);
				free(tmp0);
				free(tmp);
				break;

			case 'n':
				p++;
				if (*p != '(')
				{
					printf("ln函数使用格式不对，请重新输入。\n");
					return NULL;
				}
				//以上判断输入的是不是ln()
				//以下代码提取ln后括号内的表达式
				tmp0 = (char *)calloc(max_size, sizeof(char));
				tmp1 = (char *)calloc(max_size, sizeof(char));
				tmp = (char *)calloc(max_size, sizeof(char));

				tmp0[0] = '(';
				j = 1;
				cnt = 1; //cnt记录内部左右括号的配对
				p++;
				while (1)
				{
					while (*p == ' ')
						p++;

					if (*p == '(')
						cnt++;
					else if (*p == ')')
					{
						cnt--; //如果ln括号里面出现右括号，让它与左括号配对
						if (cnt == 0)
						{
							tmp0[j] = *p;
							j++;
							p++;
							break;
						}
					}
					tmp0[j] = *p;
					j++;
					p++;
				}
				tmp0[j] = '#';
				if (*cal(tmp0) <= 0)
				{
					printf("ln函数的参数必须大于0，请重新输入。\n");
					return NULL;
				}
				tmp_res = log(*cal(tmp0)) / log(e); //递归，计算括号内表达式的值

				tmp[0] = '(';
				if (tmp_res < 0)
					tmp[1] = '0';
				sprintf(tmp1, "%.6lf", tmp_res);

				strcat(tmp, tmp1);
				strcat(tmp, ")");
				strcat(str1, tmp);
				i += strlen(tmp);
				free(tmp0);
				free(tmp);
				free(tmp1);
				break;
			}
			break;

		case '-':
			tmp = (char *)calloc(max_size, sizeof(char));
			tmp_p = p;
			if (p != str)
				p--;
			while (p != str && *p == ' ') //退回上一个空格前的字符
				p--;
			if (p == str && !(*p == '(' || (*p >= 48 && *p < 58)))
			{
				strcat(tmp, "0-");
				p++;
			}
			else
			{
				if (*p == '(')
					strcat(tmp, "0-");
				else
					strcat(tmp, "-");
			}

			p = tmp_p; //p回位
			p++;	   //减号已处理，跳过

			strcat(str1, tmp);
			i += strlen(tmp);
			free(tmp);
			break;

		case '/': //检验是否除以0
			strcat(str1, "/");
			i++;
			p++;
			while (*p == ' ')
				p++; //跳过空格
			tmp = (char *)calloc(max_size, sizeof(char));
			tmp1 = (char *)calloc(max_size, sizeof(char));
			tmp2 = (char *)calloc(max_size, sizeof(char));
			cnt1 = 0;
			if ((*p >= 48 && *p < 58)) //如果除号后面是没有括号的数字，直接把数字提取出来
			{
				for (j = 0; (*p >= 48 && *p < 58) || *p == '.'; p++, j++)
					tmp1[j] = *p; //if *p is a number or '.'
			}
			else if (*p == '(') //如果是括号，提取该括号里的内容
			{
				tmp1[0] = '(';
				j = 1;
				cnt = 1; //cnt记录内部左右括号的配对
				p++;
				while (1)
				{
					while (*p == ' ')
						p++;

					if (*p == '(')
						cnt++;
					else if (*p == ')')
					{
						cnt--; //如果括号里面出现右括号，让它与左括号配对
						if (cnt == 0)
						{
							tmp1[j] = *p;
							j++;
							p++;
							break;
						}
					}
					tmp1[j] = *p;
					j++;
					p++;
				}
			}
			else //最后一种情况只能是函数，而且是单个函数
			{
				flag = 0;
				while (flag != 1 || cnt1 != 0)
				{
					if (*p == '(')
					{
						flag = 1;
						cnt1++;
					}
					else if (*p == ')')
						cnt1--;
					tmp1[j] = *p;
					j++;
					p++;
				}
			}

			tmp1[j] = '#';
			tmp_res = *cal(tmp1);
			if (tmp_res == 0) //如果被除数是0，报错
			{
				printf("除号后的结果不能是0，请重新输入。\n");
				return NULL;
			}
			tmp[0] = '(';
			if (tmp_res < 0)
				tmp[1] = '0';
			sprintf(tmp2, "%.6lf", tmp_res);
			strcat(tmp, tmp2);
			strcat(tmp, ")");
			strcat(str1, tmp);
			i += strlen(tmp);
			free(tmp);
			free(tmp1);
			free(tmp2);
			break;

		default:
			str1[i] = *p;
			i++;
			p++;
			break;
		}
	}
	str1[i] = '#';
	return str1;
}

char *in_to_suf(char *str)
{
	char *p, *line, *str1, *tmp_p;
	char *tmp, *tmp1, *tmp2, *tmp0;
	double tmp_res1, tmp_res2, tmp_res;
	int i, j, cnt, cnt1, flag;
	
	stackADT stack;
	stack = NewStack();
	
	line = (char *)calloc(max_size, sizeof(char));
	str1 = cal_fun(str);
	if(str1 == NULL)
		return NULL;
	p = str1;

	while (*p != '#')
	{
		switch (*p)
		{
		case ' ': //if *p is a blank, skip it.
			while (*p == ' ')
				p++;
			break;

		case '(':
			tmp = (char *)calloc(2, sizeof(char));
			*tmp = *p;
			PushStack(stack, (void *)tmp);
			p++;
			break;
		case ')': //将栈中左括号之后入栈的运算符全部弹栈输出，同时左括号出栈但是不输出。
			tmp = (char *)PopStack(stack);
			tmp[1] = '\0';
			while (*tmp != '(')
			{
				strcat(line, tmp);
				strcat(line, " ");
				tmp = (char *)PopStack(stack);
				tmp[1] = '\0';
			}
			p++;
			free(tmp);
			break;

		case '*':
			while (!IsemptyStack(stack))
			{
				tmp = (char *)PopStack(stack);
				if (*tmp == '*' || *tmp == '/')
				{
					tmp[1] = '\0';
					strcat(line, tmp);
					strcat(line, " ");
				}
				else
				{
					PushStack(stack, (void *)tmp);
					break;
				}
			}
			tmp = (char *)calloc(2, sizeof(char));
			*tmp = *p;
			PushStack(stack, (void *)tmp);
			p++;
			break;
		case '/':
			while (!IsemptyStack(stack))
			{
				tmp = (char *)PopStack(stack);
				if (*tmp == '*' || *tmp == '/')
				{
					tmp[1] = '\0';
					strcat(line, tmp);
					strcat(line, " ");
				}
				else
				{
					PushStack(stack, (void *)tmp);
					break;
				}
			}
			tmp = (char *)calloc(2, sizeof(char));
			*tmp = *p;
			PushStack(stack, (void *)tmp);
			p++;
			break;

		case '+':
			while (!IsemptyStack(stack))
			{
				tmp = (char *)PopStack(stack);
				if (*tmp != '(')
				{
					tmp[1] = '\0';
					strcat(line, tmp);
					strcat(line, " ");
				}
				else
				{
					PushStack(stack, (void *)tmp);
					break;
				}
			}
			tmp = (char *)calloc(2, sizeof(char));
			*tmp = *p;
			PushStack(stack, (void *)tmp);
			p++;

			break;
		case '-':
			while (!IsemptyStack(stack))
			{
				tmp = (char *)PopStack(stack);
				if (*tmp != '(')
				{
					tmp[1] = '\0';
					strcat(line, tmp);
					strcat(line, " ");
				}
				else
				{
					PushStack(stack, (void *)tmp);
					break;
				}
			}
			tmp = (char *)calloc(2, sizeof(char));
			*tmp = *p;
			PushStack(stack, (void *)tmp);
			p++;

			break;

		default:
			if (!(*p >= 48 && *p < 58))
			{
				printf("无法识别部分内容，请重新输入。\n");
				return NULL;
			}
			tmp = (char *)calloc(max_size, sizeof(char));
			for (j = 0; (*p >= 48 && *p < 58) || *p == '.'; p++, j++)
				tmp[j] = *p; //if *p is a number or '.'
			tmp[j] = '\0';	 //tmp[] reads the real number
			strcat(line, tmp);
			strcat(line, " ");
			free(tmp);
			break;
		}
	}

	tmp1 = (char *)calloc(max_size, sizeof(char));
	for (j = 0; !IsemptyStack(stack); j++)
	{
		tmp2 = (char *)PopStack(stack);
		tmp1[j] = *tmp2;
		j++;
		tmp1[j] = ' ';
	}

	strcat(line, tmp1);
	free(tmp1);
	strcat(line, "#");
	return line;
}

double cal_suf(char *line)
{
	char tmp[max_size];
	char *p;
	stackADT stack;
	double *pnum[max_size], *left, *right, *result[max_size];
	int i, j, k;

	double *tmp0;
	p = line; //use p to traverse the suffix expression.

	stack = NewStack();
	j = 0, k = 0;	  //j is used to count stack->array[]
	while (*p != '#') //k is used to count pnum[] and result[]
	{
		switch (*p)
		{
		case ' ': //if *p is a blank, skip it.
			while (*p == ' ')
				p++;
			break;

		case '+':
			//result[k] records the Intermediate results.
			//Not all the points in result[] are used.
			result[k] = New(double *);

			//pop two elements
			right = (double *)PopStack(stack);
			left = (double *)PopStack(stack);

			*result[k] = *left + *right;		 //caculate this two elements.
			PushStack(stack, (void *)result[k]); //push the result into stack.
			j--;								 //pop twice and push once, in total minus 1.
			p++;
			break;

		case '-':
			result[k] = New(double *);

			right = (double *)PopStack(stack); //pop two elements
			left = (double *)PopStack(stack);

			*result[k] = *left - *right;		 //caculate this two elements
			PushStack(stack, (void *)result[k]); //push the result into stack
			j--;								 //pop twice and push once, in total minus 1
			p++;
			break;

		case '*':
			result[k] = New(double *);

			right = (double *)PopStack(stack);
			left = (double *)PopStack(stack);

			*result[k] = (*left) * (*right);	 //caculate this two elements
			PushStack(stack, (void *)result[k]); //push the result into stack
			j--;								 //pop twice and push once, in total minus 1
			p++;
			break;

		case '/':
			result[k] = New(double *);

			right = (double *)PopStack(stack);
			left = (double *)PopStack(stack);

			*result[k] = *left / *right;
			PushStack(stack, (void *)result[k]);
			j--; //pop twice and push once, in total minus 1
			p++;
			break;

		default: //the ASCII code of '0' is 48.
			for (i = 0; (*p >= 48 && *p < 58) || *p == '.'; p++, i++)
				tmp[i] = *p; //if *p is a number or '.'
			tmp[i] = '\0';	 //tmp[] reads the real number
							 //pnum[k] records it.
			pnum[k] = New(double *);
			*pnum[k] = StringToReal(tmp);

			PushStack(stack, (void *)pnum[k]); //push the real num into stack

			j++, k++; //j is used to count stack->array[]
		}			  //k is used to count pnum[] and result[]
	}

	tmp0 = (double *)stack->array[0]; //stack->array[0] is the final answer.

	double res;
	res = *tmp0;
	FreeStack(stack);
	return res;
}


